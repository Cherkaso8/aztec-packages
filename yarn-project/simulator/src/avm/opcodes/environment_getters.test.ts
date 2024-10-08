import { GasFees } from '@aztec/circuits.js';
import { FunctionSelector as FunctionSelectorType } from '@aztec/foundation/abi';
import { AztecAddress } from '@aztec/foundation/aztec-address';
import { Fr } from '@aztec/foundation/fields';

import { randomInt } from 'crypto';

import { type AvmContext } from '../avm_context.js';
import { TypeTag } from '../avm_memory_types.js';
import { initContext, initExecutionEnvironment, initGlobalVariables } from '../fixtures/index.js';
import { Opcode } from '../serialization/instruction_serialization.js';
import { EnvironmentVariable, GetEnvVar } from './environment_getters.js';

describe('Environment getters', () => {
  const address = AztecAddress.random();
  const storageAddress = AztecAddress.random();
  const sender = AztecAddress.random();
  const functionSelector = FunctionSelectorType.random();
  const transactionFee = Fr.random();
  const chainId = Fr.random();
  const version = Fr.random();
  const blockNumber = Fr.random();
  const timestamp = new Fr(randomInt(100000)); // cap timestamp since must fit in u64
  const feePerDaGas = Fr.random();
  const feePerL2Gas = Fr.random();
  const isStaticCall = true;
  const gasFees = new GasFees(feePerDaGas, feePerL2Gas);
  const globals = initGlobalVariables({
    chainId,
    version,
    blockNumber,
    timestamp,
    gasFees,
  });
  const env = initExecutionEnvironment({
    address,
    storageAddress,
    sender,
    functionSelector,
    transactionFee,
    globals,
    isStaticCall,
  });
  let context: AvmContext;
  beforeEach(() => {
    context = initContext({ env });
  });

  it(`Should (de)serialize correctly`, () => {
    const buf = Buffer.from([
      Opcode.GETENVVAR_16, // opcode
      0x01, // indirect
      0x05, // var idx
      ...Buffer.from('1234', 'hex'), // dstOffset
    ]);
    const instr = new GetEnvVar(/*indirect=*/ 0x01, 5, /*dstOffset=*/ 0x1234).as(
      Opcode.GETENVVAR_16,
      GetEnvVar.wireFormat16,
    );

    expect(GetEnvVar.as(GetEnvVar.wireFormat16).deserialize(buf)).toEqual(instr);
    expect(instr.serialize()).toEqual(buf);
  });

  describe.each([
    [EnvironmentVariable.ADDRESS, address.toField()],
    [EnvironmentVariable.STORAGEADDRESS, storageAddress.toField()],
    [EnvironmentVariable.SENDER, sender.toField()],
    [EnvironmentVariable.FUNCTIONSELECTOR, functionSelector.toField(), TypeTag.UINT32],
    [EnvironmentVariable.TRANSACTIONFEE, transactionFee.toField()],
    [EnvironmentVariable.CHAINID, chainId.toField()],
    [EnvironmentVariable.VERSION, version.toField()],
    [EnvironmentVariable.BLOCKNUMBER, blockNumber.toField()],
    [EnvironmentVariable.TIMESTAMP, timestamp.toField(), TypeTag.UINT64],
    [EnvironmentVariable.FEEPERDAGAS, feePerDaGas.toField()],
    [EnvironmentVariable.FEEPERL2GAS, feePerL2Gas.toField()],
    [EnvironmentVariable.ISSTATICCALL, new Fr(isStaticCall ? 1 : 0)],
  ])('Environment getter instructions', (envVar: EnvironmentVariable, value: Fr, tag: TypeTag = TypeTag.FIELD) => {
    it(`Should read '${EnvironmentVariable[envVar]}' correctly`, async () => {
      const instruction = new GetEnvVar(/*indirect=*/ 0, envVar, /*dstOffset=*/ 0);

      await instruction.execute(context);

      expect(context.machineState.memory.getTag(0)).toBe(tag);
      const actual = context.machineState.memory.get(0).toFr();
      expect(actual).toEqual(value);
    });
  });
});