#include <iostream>
#include <string>
#include <bitset>
using namespace std;
unsigned PC, next_PC_val, opr1, opr2, opr3, rs_val, rt_val, rd_val, Memdata,
opcode, rs, rt, rd,
shamt = 0, funct, jsec, type, dontcare = 'x', choice, ALUresult,
MUX_Out, Zero, Add_result, ALUControlOUT, MUX0, MUX3, MUX1, MUX2, ReadData,
tempPcnextbranch;
uint16_t offset; string opName, InstructiontoIFIDbuffer,
InstructionIDEXBuffer, instructionExMEMBuffer, instructionMEMWBBuffer;
bitset<6> op_bin, funct_bin; bitset<5> rs_bin, rt_bin, rd_bin, shamt_bin;
bitset<16> offset_bin; bitset<32> next_PC_val_bin, offsetSignExt_bin;
void STARTOVER();
void invokeTraceChoice();
unsigned PCAdder(unsigned pc) {
next_PC_val = pc + int(4);
return next_PC_val;
}
// Reads the instruction and tokenizes the string
void instruction_ConsoleInput() {
string instr;
int i = 0;
string::size_type sz;
cout << "\nEnter the command as per input format!! \n 1. Order of operands
as per MIPS Command\n 2. Please make sure to use Registers ranging from 0
to 31 and memory less than 4294967292(2^32-4)\nIntruction:::" ;
getline(cin, instr);
string token, mipsInstr(instr);
while (mipsInstr[0] == ' ' || token != mipsInstr) {
if (mipsInstr[0] != ' ') {
token = mipsInstr.substr(0, mipsInstr.find_first_of( " "));
i++;
}
mipsInstr = mipsInstr.substr(mipsInstr.find_first_of( " ") + 1);
if (i == 1) { // operation name
opName = token.c_str();
}
if (i == 2) { //Register value
opr1 = std::stoi(token.c_str(), &sz);
}
if (i == 3) { //Register/offset value based on
type of instruction
opr2 = std::stoi(token.c_str(), &sz);
}
if (i == 4) { //Register value
opr3 = std::stoi(token.c_str(), &sz);
}
}
if (opName == "add" || opName == "sub") {
if (opr1 > 31 || opr2 > 31 || opr3 > 31) {
cout << "\nRegister chosen is out of range specified, lets start
over again\n\n";
STARTOVER();
}
} else if (opName == "lw" || opName == "sw") {
if (opr1 > 31 || opr3 > 31) {
cout << "\nRegister chosen is out of range specified, lets start
over again\n\n";
STARTOVER();
}
} else if (opName == "beq") {
if (opr1 > 31 || opr2 > 31) {
cout << "\nRegister chosen is out of range specified, lets
start over again\n\n";
STARTOVER();
}
} else {
cout << "\nInvalid operation name specified, lets start over again\n
\n";
}
}
int getPCAddress() {
cout << " Welcome!! You can now trace path for one of the following
MIPS Command:::add, sub, lw, sw, beq \n ";
instruction_ConsoleInput();
cout << "Enter current Program counter value::" ;
cin >> PC;
if (PC % 4 != 0 || PC > (4294967296 - 4)) {
cout << "\nMemory address should be multiple of 4 and less than
4294967292 for MIPS, please begin again:: \n" ;
STARTOVER();
}
PCAdder(PC);
return PC;
}
void traceFetchStage() {
cout << "\n\n:::::FetchStage::::" ;
cout << "\nDatapath-> Read Address/Adder IN1::" << PC;
cout << "\nDatapath-> Adder IN2::" << int(4);
cout << "\nDatapath-> Adder OUT/MUX0 port0 IN::" << next_PC_val;
cout << "\nDatapath-> MUX0 port1 IN undecided until MEM stage is reached" ;
cout << "\nDatapath-> MUX0 Selector line undecided until EX/MEM buffer is
loaded";
cout << "\nInstruction IF/ID Buffer in Decimal::::" <<
InstructiontoIFIDbuffer;
}
void loadInstructiontoIFIDbuffer() {
if (type == 1) {
InstructiontoIFIDbuffer = to_string(next_PC_val) + "|" + to_string
(opcode) + "|" + to_string(rs) + "|" + to_string(rt) + "|" +
to_string(rd)
+ "|" + to_string(shamt) + "|" + to_string(funct);
}
else if (type == 2) {
InstructiontoIFIDbuffer = to_string(next_PC_val) + "|" + to_string
(opcode) + "|" + to_string(rs) + "|" + to_string(rt) + "|" +
to_string(offset);
}
}
/* instruction partition */
void FetchStage(string operation, unsigned opr1, unsigned opr2, unsigned opr3)
{
// Partition instruction into several parts (op, r1, r2, r3, funct, offset)
/*values set for instructions in decimal*/
if (operation == "add" || operation == "sub") {
cout << "\nLoaded Instruction is R-type" ;
opcode = 0;
rs = opr2;
rt = opr3;
rd = opr1;
rd_bin = bitset<5>(rd);
shamt_bin = bitset<5>(0);
type = 1;// R - type intruction, identifier
if (operation == "add") {
funct = 32;
funct_bin = bitset<6>(funct);
string offsetTemp = rd_bin.to_string() + shamt_bin.to_string()
+ funct_bin.to_string();
bitset<16> offsetTemp_bin(offsetTemp);
offset = int(offsetTemp_bin.to_ulong());
offset_bin = offsetTemp_bin;
cout << "\nSymantics of instruction entered::" << "$R" +
to_string(rd)+" <-- $R" + to_string(rs) + " + $R" + to_string
(rt);
cout << "\nEnter value for " << "R" + to_string(rs) << ":: ";
cin >> rs_val;
cout << "Enter value for " << "R" + to_string(rt) << ":: ";
cin >> rt_val;
} else {
funct = 34;
funct_bin = bitset<6>(funct);
string offsetTemp = rd_bin.to_string() + shamt_bin.to_string()
+ funct_bin.to_string();
bitset<16> offsetTemp_bin(offsetTemp);
offset = int(offsetTemp_bin.to_ulong());
offset_bin = offsetTemp_bin;
cout << "\nSymantics of instruction entered::" << "$R" +
to_string(rd) + " <-- $R" + to_string(rs) + " - $R" +
to_string(rt);
cout << "\nEnter value for " << "R" + to_string(rs) << ":: ";
cin >> rs_val;
cout << "Enter value for " << "R" + to_string(rt) << ":: ";
cin >> rt_val;
}
}
else if (operation == "lw") {
cout << "\nLoaded Instruction is I-type " ;
opcode = 35;
rs = opr3;
rt = opr1;
rd = dontcare;
offset = opr2;
bitset<16> offsetTemp_bin(offset);
offset = int(offsetTemp_bin.to_ulong());
offset_bin = offsetTemp_bin;
funct = dontcare;
cout << "\nSymantics of instruction entered::" << "$R" + to_string
(rt) + " <-- Mem[$R" + to_string(rs) + " + " + to_string(offset)
+ "]";
type = 2;// I - type intruction, identifier
cout << "\nEnter value for " << "R" + to_string(rs) << ":: ";
cin >> rs_val;
cout << "Enter value that is loaded in final memory location:: " ;
cin >> Memdata;
}
else if (operation == "sw") {
cout << "\nLoaded Instruction is I-type " ;
opcode = 43;
rs = opr3;
rt = opr1;
rd = dontcare;
offset = opr2;
bitset<16> offsetTemp_bin(offset);
offset = int(offsetTemp_bin.to_ulong());
offset_bin = offsetTemp_bin;
funct = dontcare;
cout << "\nSymantics of instruction entered::" << "Mem[$R" +
to_string(rs) + " + " + to_string(offset) + "] <-- $R" +
to_string(rt);
type = 2;// I - type intruction, identifier
cout << "\nEnter value for " << "R" + to_string(rs) << ":: ";
cin >> rs_val;
cout << "Enter value for " << "R" + to_string(rt) << ":: ";
cin >> rt_val;
}
else if (operation == "beq") {
cout << "\nLoaded Instruction is I-type " ;
opcode = 4;
rs = opr1;
rt = opr2;
rd = dontcare;
offset = opr3;
funct = dontcare;
cout << "\nif ($R" + to_string(rs) + "== $R" + to_string(rt) + ")
go to PC+4+" + to_string(offset) + "*4";
type = 2; // I- type intruction, identifier
cout << "\nEnter value for " << "R" + to_string(rs) << ":: ";
cin >> rs_val;
cout << "Enter value for " << "R" + to_string(rt) << ":: ";
cin >> rt_val;
}
loadInstructiontoIFIDbuffer();
invokeTraceChoice();
if (choice == 0 || choice == 1) {
traceFetchStage();
}
}
/* Declare control vector*/
typedef struct
{
char RegDst;
char ALUOp1;
char ALUOp0;
char ALUSrc;
char Branch;
char MemRead;
char MemWrite;
char RegWrite;
char MemtoReg;
}struct_controls;
struct_controls controls;
/* Define control vector*/
int controlSignals(unsigned op, struct_controls *controls) {
switch (op) {
case 0:
//Register Type:
/*opcode is (0) which means it is an R -type instruction, the RegDst is
on(1) because there is a register destination in use, and RegWrite is
on(1) because a register is also being written, the ALUSrc is zero
because the second operand for the ALU is taken from a register*/
controls->RegDst = 1;
controls->ALUOp1 = 1;
controls->ALUOp0 = 0;
controls->ALUSrc = 0;
controls->Branch = 0;
controls->MemRead = 0;
controls->MemWrite = 0;
controls->RegWrite = 1;
controls->MemtoReg = 0;
break;
case 35:
// Load word:
/*MemRead is on(1) because there is date read from memory, MemtoReg is
on(1) because the memory is written to a register after it is loaded,
ALUSrc is 1 because the instruction is an immediate type, RegWrite
is on because a register is being written, ALUop is 0 */
controls->RegDst = 0;
controls->ALUOp1 = 0;
controls->ALUOp0 = 0;
controls->ALUSrc = 1;
controls->Branch = 0;
controls->MemRead = 1;
controls->MemWrite = 0;
controls->RegWrite = 1;
controls->MemtoReg = 1;
break;
case 43:
// Store word:
/*MemWrite is on(1) because memory is being written, ALUSrc is 1
because the second operand for the ALU is taken from the instruction,
ALUop is 0 */
controls->RegDst = 0;
controls->ALUOp1 = 0;
controls->ALUOp0 = 0;
controls->ALUSrc = 1;
controls->Branch = 0;
controls->MemRead = 0;
controls->MemWrite = 1;
controls->RegWrite = 0;
controls->MemtoReg = 0;
break;
case 4:
// Branch Eq:
/*Branch is on(1) because the instruction is a branch and the equal
comparator is used, ALUSrc is 1 because the second operand for the
ALU is in the instruction */
//cout << "\n I came here";controls->RegDst = 0;
controls->ALUOp1 = 0;
controls->ALUOp0 = 1;
controls->ALUSrc = 0;
controls->Branch = 1;
controls->MemRead = 0;
controls->MemWrite = 0;
controls->RegWrite = 0;
controls->MemtoReg = 0;
break;
}
return 0;
}void traceDecodeStage() {
cout << "\n\n:::::DECODE STAGE::::: " ;
cout << "\nInstruction IF/ID Buffer in Decimal::::" <<
InstructiontoIFIDbuffer;
cout << "\nDatapath--> Read register 1::" << rs;
cout << "\nDatapath--> Read register 2::" << rt;
cout << "\nDatapath-->instruction[15..0]::" << offset_bin.to_string();
cout << "\nDatapath-->instruction[20..16]::" << rt_bin.to_string();
cout << "\nDatapath-->instruction[15..11]::" << rd_bin.to_string();
cout << "\nDatapath-->next PC value::" << next_PC_val;
cout << "\nDatapath-->Read Data 1::" << rs_val;
cout << "\nDatapath-->Read Data 2::" << rt_val;
cout << "\nDatapath-->offset before sign extension ::" << bitset<16>
(offset);
cout << "\nDatapath-->offset after sign extension ::" <<
offsetSignExt_bin.to_string() << " :: Decimal Equivalent ::" << int
(offsetSignExt_bin.to_ulong());
cout << "\nControl Signal::::::::" << to_string(controls.RegDst) +
to_string(controls.ALUOp1) + to_string(controls.ALUOp0) + to_string
(controls.ALUSrc) <<
to_string(controls.Branch) + to_string(controls.MemRead) +
to_string(controls.MemWrite) + to_string(controls.RegWrite) +
to_string(controls.MemtoReg);
cout << "\nInstruction ID/EX Buffer in Decimal:::: " <<
InstructionIDEXBuffer;
}
void loadInstructiontoIDEXbuffer() {
InstructionIDEXBuffer = to_string(controls.RegDst) + to_string
(controls.ALUOp1) + to_string(controls.ALUOp0) + to_string
(controls.ALUSrc) +
to_string(controls.Branch) + to_string(controls.MemRead) + to_string
(controls.MemWrite) + to_string(controls.RegWrite) + to_string
(controls.MemtoReg)
+ "|" + to_string(next_PC_val) + "|" + to_string(rs_val) + "|" +
to_string(rt_val) + "|" + to_string(offset) + "|" + to_string(rt) +
"|" + to_string(rd);
}
void DecodeStage() {
rt_bin = bitset<5>(rt);
offset_bin = bitset<16>(offset);
offsetSignExt_bin = bitset<32>(offset);
if (type == 2) { // I-type instruction, split rd from offset
unsigned upper5bitsOffset = (offset >> 10) & 0x1F;
rd = upper5bitsOffset;
}
rd_bin = bitset<5>(rd);
controlSignals(opcode, &controls); // based on opcode load control signal
loadInstructiontoIDEXbuffer();
if (choice == 0 || choice == 2) {
traceDecodeStage();
}
}
/* ALU */
void ALU(unsigned A, unsigned B, char ALUOp)
{
switch (ALUOp) {
case 0: // Addition
ALUresult = A + B;
break;
case 1: // Subtraction
ALUresult = A - B;
break;
}
if (ALUresult == 0) {
Zero = 1; // Set Zero based on results. 1 if ALUResult = 0;
otherwise, 0.
} else {
Zero = 0;
}
}
/*Utility program for mux's in mips*/
int twoInputMUX(int cntrlsignal, int port0, int port1) {
if (cntrlsignal == 0) {
MUX_Out = port0;
}
else if (cntrlsignal == 1) {
MUX_Out = port1;
}
return MUX_Out;
}
/*calculate program counter's relative address incase of branching*/
int relative_PCAddress() {
	offsetSignExt_bin = offsetSignExt_bin << 2;
Add_result = next_PC_val + int(offsetSignExt_bin.to_ulong());
return Add_result;
}
/*Trace ExecuteStage*/
void traceExecuteStage() {
cout << "\n\n:::::EXECUTE STAGE::::: " ;
cout << "\nInstruction ID/EX Buffer in Decimal:::: " <<
InstructionIDEXBuffer;
cout << "\nDatapath->MUX1 port0::" << rt;
cout << "\nDatapath->MUX1 port1::" << rd;
cout << "\nDatapath->Control input MUX1 (RegDst)::" << to_string
(controls.RegDst);
cout << "\nDatapath->MUX1 output::::" << MUX1;
cout << "\nDatapath->ALUOp1::" << to_string(controls.ALUOp1);
cout << "\nDatapath->ALUOp0::" << to_string(controls.ALUOp0);
cout << "\nDatapath->ALUControlIN_1 (6 bit funct value)::" <<
funct_bin;
cout << "\nDatapath->ALUControlIN_2::" << to_string(controls.ALUOp1) <<
to_string(controls.ALUOp0);
cout << "\nDatapath->ALUControlOUT::" << ALUControlOUT;
cout << "\nDatapath->MUX2 port0::" << rt_val;
cout << "\nDatapath->MUX2 port1::" << int(offsetSignExt_bin.to_ulong
());
cout << "\nDatapath->Control input MUX2 (ALUSrc)::" << to_string
(controls.ALUSrc);
cout << "\nDatapath->MUX2 output::::" << MUX2;
cout << "\nDatapath->ALU_IN1::" << rs_val;
cout << "\nDatapath->ALU_IN2::" << MUX_Out;
cout << "\nDatapath->ALUResult::" << ALUresult;
cout << "\nDatapath->Zero Flag::" << Zero;
cout << "\nDatapath-> Adder_IN1 ::" << next_PC_val;
cout << "\nDatapath-> Adder_IN2(after shift left 2) ::" <<
offsetSignExt_bin.to_string() << " || Decimal value::" << int
(offsetSignExt_bin.to_ulong());
cout << "\nDatapath-> Add_result ::" << Add_result;
cout << "\nInstruction EX/MEM Buffer in Decimal:::: " <<
instructionExMEMBuffer;
}
void loadInstructiontoEXMEMbuffer() {
instructionExMEMBuffer = to_string(controls.Branch) + to_string
(controls.MemRead) + to_string(controls.MemWrite)
+ to_string(controls.RegWrite) + to_string(controls.MemtoReg) + "|" +
to_string(Add_result) + "|" + to_string(Zero)
+ "|" + to_string(ALUresult) + "|" + to_string(rt_val) + "|" +
to_string(MUX1);
}
/*Execute stage operations*/
int ExecuteStage(unsigned data1, unsigned data2, unsigned extended_value,
unsigned funct, char ALUSrc, unsigned ALUresult, char Zero) {
MUX1 = twoInputMUX(controls.RegDst, rt, rd);
MUX2 = twoInputMUX(controls.ALUSrc, rt_val, int(offsetSignExt_bin.to_ulong
()));
if (to_string(controls.ALUOp1) + to_string(controls.ALUOp0) == "10" ||
to_string(controls.ALUOp1) + to_string(controls.ALUOp0) == "01") {
switch (opcode) {
case 0:
if (funct == 32) { // Addition unsigned
ALUControlOUT = 0;
} else if (funct == 34) { // Subtraction unsigned
ALUControlOUT = 1;
}
break;
case 4: // beq
ALUControlOUT = 1;
break;
default: // for others
ALUControlOUT = 0;
break;
}
} else {
ALUControlOUT = 0;
}
MUX_Out = twoInputMUX(controls.ALUSrc, rt_val, int
(offsetSignExt_bin.to_ulong()));
ALU(rs_val, MUX_Out, ALUControlOUT);
relative_PCAddress();
loadInstructiontoEXMEMbuffer();
if (choice == 0 || choice == 3) {
traceExecuteStage();
}
return 0;
}
int cntrlSignalMux0(char branch, int zero) {
int temp = int(branch) & zero;
return temp;
}
/*Trace MEMStage*/
void traceMEMStage() {
cout << "\n\n:::::MEM STAGE::::: " ;
cout << "\nInstruction EX/MEM Buffer in Decimal:::: " <<
instructionExMEMBuffer;
cout << "\nDatapath->Branch IN1::" << to_string(controls.Branch);
cout << "\nDatapath->Branch IN2::" << Zero;
cout << "\nDatapath->Branch OUT::" << tempPcnextbranch;
cout << "\nDatapath-> MUX0 port0 IN::" << next_PC_val;
cout << "\nDatapath-> MUX0 port1 IN::" << Add_result;
cout << "\nDatapath-> Branch OUT/MUX0 Selector::" << tempPcnextbranch;
cout << "\nDatapath->Latest PC value based on branch condition::" <<
PC;
cout << "\nDatapath->Read Address::" << ALUresult;
cout << "\nDatapath->Write Address::" << ALUresult;
cout << "\nDatapath->Write data::" << rt_val;
cout << "\nDatapath->MemWrite::" << to_string(controls.MemWrite);
if (controls.MemWrite == 1)
cout << "\nDatapath->Writing data " << rt_val << " in Memory
location::" << ALUresult;
else
cout << "\n No write operation as MemWrite is not enabled" ;
cout << "\nDatapath->MemRead::" << to_string(controls.MemRead);
if (controls.MemRead == 1)
cout << "\nDatapath->Read data " << ReadData << " from Memory
location::" << ALUresult;
else
cout << "\n No read operation as MemRead is not enabled" ;
cout << "\nInstruction MEM/WB Buffer in Decimal:::: " <<
instructionMEMWBBuffer;
}
void loadInstructiontoMEMWBbuffer() {
instructionMEMWBBuffer = to_string(controls.RegWrite) + to_string
(controls.MemtoReg)
+ "|" + to_string(ReadData) + "|" + to_string(ALUresult) + "|" +
to_string(MUX1);
}
/* Read / Write Memory */
int MEMStage() {
tempPcnextbranch = cntrlSignalMux0(controls.Branch, Zero);
PC = twoInputMUX(tempPcnextbranch, next_PC_val, Add_result);
if (controls.MemWrite == 1 || controls.MemRead == 1) {
if (ALUresult > (4294967296 - 4)) { // If address is bad, then
start from beginning
cout << "\nYou are trying to access memory of out of range, please
start from the beginning\n\n" ;
STARTOVER();
}
else if (controls.MemWrite == 1) { //checks if MemWrite is 1. If it
is, then writes the register value in memory location
calculated.
Memdata = rt_val;
}
else if (controls.MemRead == 1) { //checks if MemRead is 1. If it
is, then read the value from memory location, pass it to output
port ReadData.
ReadData = Memdata;
}
} else {
ReadData = rand(); // default, random value read from
ReadData port.
}
loadInstructiontoMEMWBbuffer();
if (choice == 0 || choice == 4) {
traceMEMStage();
}
return 0;
}
/*Trace WriteBack Stage*/
void writeBackTrace() {
cout << "\n\n:::::Write Back STAGE::::: " ;
cout << "\nInstruction MEM/WB Buffer in Decimal:::: " <<
instructionMEMWBBuffer;
cout << "\nDatapath->RegWrite::" << to_string(controls.RegWrite);
cout << "\nDatapath->RegDst::" << to_string(controls.RegDst);
cout << "\nDatapath->Write register::" << MUX1;
cout << "\nDatapath->MUX3 port0 Input::" << ALUresult;
cout << "\nDatapath->MUX3 port1 Input::" << ReadData;
cout << "\nDatapath->MUX3 Control Signal MemtoReg::" << to_string
(controls.MemtoReg);
if ((controls.RegWrite && controls.RegDst) || (controls.MemtoReg))
cout << "\nDatapath->MUX3 out/Write data::" << MUX3;
else
cout << "\nNo Register write operation" ;
}
/* Write Register */
void writeBackStage() {
MUX3 = twoInputMUX(controls.MemtoReg, ALUresult, ReadData);
if (controls.RegWrite == 1 && controls.RegDst ==1) {
rd_val = MUX3; // Write ALU results to rd
} else if (controls.MemtoReg == 1 ) {
rt_val = MUX3; // Write memdata to rt
}
if (choice == 0 || choice == 5) {
writeBackTrace();
}
}
void invokeTraceChoice() {
cout << "\nSelect your choice to trace path stagewise or complete trace
of this command\nEnter your choice as below::\n 0.Complete Trace \n
1.FetchStage \n 2.DecodeStage \n 3.ExecuteStage \n 4.MEMStage \n
5.writeBackStage\nChoice:::" ;
cin >> choice;
}
int main () {
getPCAddress();
FetchStage(opName, opr1, opr2, opr3);
DecodeStage();
ExecuteStage(rs_val, rt_val, int(offsetSignExt_bin.to_ulong()), funct,
controls.ALUSrc, ALUresult, Zero);
MEMStage();
writeBackStage();
system("\npause");
return 0;
}
void STARTOVER() {
main();
}