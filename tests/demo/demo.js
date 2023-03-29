// https://github.com/AElfProject/aelf-sdk.js
const AElf = require("aelf-sdk");
// aelf-sdk.js dependencie @aelfqueen/protobufjs
const protoBuf = require("@aelfqueen/protobufjs");
const descriptor = require("@aelfqueen/protobufjs/ext/descriptor");

const tokenProtoBase64 = require("./token_proto_base64.json");

const aelf = new AElf(
  new AElf.providers.HttpProvider("http://18.163.40.216:8000")
);
const wallet = AElf.wallet.getWalletByPrivateKey(
  "4e83df2aa7c8552a75961f9ab9f2f06e01e0dca0203e383da5468bbbe2915c97"
);
const contractAddress = "JRmBduh4nXWi1aXgdUsj5gJrzeZb2LxmrAbf7W99faZSvoAaE",
  methodName = "Transfer",
  toAddress = "cDPLA9axUVeujnTTk4Cyr3aqRby3cHHAB6Rh28o7BRTTxi8US";

const getServicesFromFileDescriptors = (descriptors) => {
  const root = AElf.pbjs.Root.fromDescriptor(
    descriptors,
    "proto3"
  ).resolveAll();
  return descriptors.file
    .filter((f) => f.service.length > 0)
    .map((f) => {
      const sn = f.service[0].name;
      const fullName = f.package ? `${f.package}.${sn}` : sn;
      return root.lookupService(fullName);
    });
};

const getRawTx = (blockHeightInput, blockHashInput, packedInput) => {
  const rawTx = AElf.pbUtils.getTransaction(
    wallet.address,
    contractAddress,
    methodName,
    packedInput
  );
  rawTx.refBlockNumber = blockHeightInput;
  const blockHash = blockHashInput.match(/^0x/)
    ? blockHashInput.substring(2)
    : blockHashInput;
  rawTx.refBlockPrefix = Buffer.from(blockHash, "hex").slice(0, 4);
  return rawTx;
};

const handleTransaction = (height, hash, encoded) => {
  // Create transaction
  const rawTx = getRawTx(height, hash, encoded);
  // signature
  let tx = AElf.wallet.signTransaction(rawTx, wallet.keyPair);

  tx = AElf.pbUtils.Transaction.encode(tx).finish();
  if (tx instanceof Buffer) {
    return tx.toString("hex");
  }
  return AElf.utils.uint8ArrayToHex(tx);
};

const getFileDescriptorsSet = async (contractAddress) => {
  const fds = await aelf.chain.getContractFileDescriptorSet(contractAddress);
  return getServicesFromFileDescriptors(fds);
};

const fileDescriptorSetFormatter = (result) => {
  const buffer = Buffer.from(result, "base64");
  return descriptor.FileDescriptorSet.decode(buffer);
};

const getLocalFileDescriptorsSet = (base64) => {
  const fds = fileDescriptorSetFormatter(base64);
  return getServicesFromFileDescriptors(fds);
};
const getProtoFileDescriptorsSet = async () => {
  const root = (
    await protoBuf.load("./protobuf/token_contract.proto")
  ).resolveAll();
  return root.nested.token.nested;
};

const encodedTransfer = async (params) => {
  // Get the protobuf definitions related to a contract

  // 1.protobuf file descriptors from aelf-sdk.js
  // const fileDescriptors = await getFileDescriptorsSet(contractAddress);
  // const inputType =
  //   fileDescriptors[2].methods[methodName].resolve().resolvedRequestType;

  // 2.protobuf file descriptors from local protobuf base64
  // const fileDescriptors = getLocalFileDescriptorsSet(tokenProtoBase64);
  // const inputType =
  //   fileDescriptors[2].methods[methodName].resolve().resolvedRequestType;

  // 3.protobuf file descriptors from protobufjs
  const fileDescriptors = await getProtoFileDescriptorsSet();
  const inputType =
    fileDescriptors.TokenContract.methods[methodName].resolve()
      .resolvedRequestType;

  let input = AElf.utils.transform.transformMapToArray(inputType, params);

  input = AElf.utils.transform.transform(
    inputType,
    input,
    AElf.utils.transform.INPUT_TRANSFORMERS
  );

  const message = inputType.fromObject(input);
  return inputType.encode(message).finish();
};

(async () => {
  const encoded = await encodedTransfer({
    symbol: "ELF",
    to: toAddress,
    amount: "10000000",
    memo: "test",
  });
  console.log(encoded, "=====encoded");
  console.log(encoded[encoded.length-2], "=====the second to last byte");
  console.log(encoded[encoded.length-1], "=====the last byte");
  const { BestChainHeight, BestChainHash } = await aelf.chain.getChainStatus();

  const transaction = handleTransaction(
    BestChainHeight,
    BestChainHash,
    encoded
  );
  // Broadcast a transaction
  const send = await aelf.chain.sendTransaction(transaction);
  setTimeout(async () => {
    // Get the result of a transaction
    const result = await aelf.chain.getTxResult(send.TransactionId);
    console.log(result, "=====tx-result");
  }, 2000);
})();
