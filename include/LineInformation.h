#ifndef _LineInformation_h_
#define _LineInformation_h_

#include <Base.h>
#include <CStructuresDwarf.h>
#include <defines/LineInformation.d>

class LineInfoTable;
class BasicBlock;
class DwarfLineInfoSection;

static char* currentDirectory = ".";
   

class LineInfo : public Base {
private:
    void initializeWithDefaults();
    void initializeWithInstruction(char* instruction);

    void updateRegsExtendedOpcode(char* instruction);
    void updateRegsStandardOpcode(char* instruction);
    void updateRegsSpecialOpcode(char* instruction);

protected:
    uint32_t index;
    uint32_t instructionSize;
    DWARF2_LineInfo_Registers entry;

    uint8_t* instructionBytes;

    LineInfoTable* header;
public:
    LineInfo(uint32_t idx, char* instruction, LineInfoTable* hdr);
    ~LineInfo();

    LINEINFO_MACROS_CLASS("For the get_X/set_X field macros check the defines directory");    

    void print();
    char* charStream() { return (char*)&entry; }
    uint32_t getInstructionSize() { return instructionSize; }

    char* getFileName();
    char* getFilePath();

    bool verify();
    LineInfoTable* getHeader() { return header; }

    uint32_t getIndex() { return index; }
    const char* breifName() { return "LineInfo"; }
};

class LineInfoTable : public Base {
protected:
    uint32_t index;
    char* rawDataPtr;

    DWARF2_Internal_LineInfo entry;

    LineInfo** lineInformations;
    uint32_t numberOfLineInformations;

    uint8_t* opcodes;
    uint32_t numberOfOpcodes;

    char** includePaths;
    uint32_t numberOfIncludePaths;

    char** fileNames;
    uint32_t numberOfFileNames;
    uint8_t* fileDirIndices;
    uint8_t* fileModTimes;
    uint8_t* fileSizes;

    LineInfo* registers;

public: 
    LineInfoTable(uint32_t idx, char* raw);
    ~LineInfoTable();

    void print();
    uint32_t read(BinaryInputFile* b);
    void dump(BinaryOutputFile* b, uint32_t offset);
    bool verify();

    LINEINFOTABLE_MACROS_CLASS("For the get_X/set_X field macros check the defines directory");

    const char* breifName() { return "LineInfoTable"; }

    LineInfo* getRegisters() { return registers; }
    uint32_t getOpcodeLength(uint32_t idx);

    uint32_t getNumberOfLineInfos() { return numberOfLineInformations; }
    LineInfo* getLineInfo(uint32_t idx) { ASSERT(idx < numberOfLineInformations); return lineInformations[idx]; }

    char* getFileName(uint32_t idx);
    char* getIncludePath(uint32_t idx);
};

class LineInfoFinder {
protected:
    LineInfo** sortedLineInfos;
    uint32_t numberOfLineInfos;

    DwarfLineInfoSection* dwarfLineInfoSection;
public:
    LineInfoFinder(DwarfLineInfoSection* dwarfLineSection);
    ~LineInfoFinder();

    LineInfo* lookupLineInfo(BasicBlock* bb);
    LineInfo* lookupLineInfo(uint64_t addr);

    const char* briefName() { return "LineInfoFinder"; }

    bool verify();
};


#endif /* _LineInformation_h_ */
