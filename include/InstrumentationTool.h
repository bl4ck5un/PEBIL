/* 
 * This file is part of the pebil project.
 * 
 * Copyright (c) 2010, University of California Regents
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _InstrumentationTool_h_
#define _InstrumentationTool_h_

#include <ElfFileInst.h>
#include <Instrumentation.h>
#include <X86Instruction.h>
#include <map>

class InstrumentationPoint;

#define INFO_UNKNOWN "__info_unknown__"

typedef struct 
{
    int64_t pt_vaddr;
    int64_t pt_target;
    int64_t pt_flags;
    int32_t pt_size;
    int32_t pt_blockid;
    unsigned char pt_content[16];
    unsigned char pt_disable[16];
} instpoint_info;

class InstrumentationTool : public ElfFileInst {
private:
    char* extension;
    bool singleArgCheck(void* arg, uint32_t mask, const char* name);

protected:
    uint64_t imageKey;
    uint64_t threadHash;

    Vector<X86Instruction*>* atomicIncrement(uint32_t dest, uint32_t scratch, uint32_t count, uint64_t memaddr, Vector<X86Instruction*>* insns);

    void printStaticFile(Vector<BasicBlock*>* allBlocks, Vector<uint32_t>* allBlockIds, Vector<LineInfo*>* allBlockLineInfos, uint32_t bufferSize);
    void printStaticFilePerInstruction(Vector<X86Instruction*>* allInstructions, Vector<uint32_t>* allInstructionIds, Vector<LineInfo*>* allInstructionLineInfos, uint32_t bufferSize);

    InstrumentationPoint* insertBlockCounter(uint64_t, Base*);
    InstrumentationPoint* insertBlockCounter(uint64_t, Base*, bool, uint32_t);
    InstrumentationPoint* insertBlockCounter(uint64_t, Base*, bool, uint32_t, uint32_t);
    InstrumentationPoint* insertInlinedTripCounter(uint64_t, X86Instruction*, bool, uint32_t, InstLocations, BitSet<uint32_t>*, uint32_t val);

    void assignStoragePrior(InstrumentationPoint* pt, uint32_t value, uint64_t address, uint8_t tmpreg, uint64_t regbak);
    Vector<X86Instruction*>* storeThreadData(uint32_t scratch, uint32_t dest);
    Vector<X86Instruction*>* storeThreadData(uint32_t scratch, uint32_t dest, bool storeToStack, uint32_t stackPatch);
    void threadAllEntryPoints(Function* f, uint32_t threadReg);

    std::map<uint64_t, uint32_t>* threadReadyCode();
    uint32_t instrumentForThreading(Function* func);

    InstrumentationFunction* imageInit;
    InstrumentationFunction* initWrapperC;
    InstrumentationFunction* initWrapperF;

    uint32_t phaseNo;
    bool loopIncl;
    bool printDetail;
    char* inputFile;
    char* dfpFile;
    char* trackFile;
    bool doIntro;

#define PEBIL_OPT_ALL 0xffffffff
#define PEBIL_OPT_NON 0x00000000
#define PEBIL_OPT_PHS 0x00000001
#define PEBIL_OPT_LPI 0x00000002
#define PEBIL_OPT_DTL 0x00000004
#define PEBIL_OPT_INP 0x00000008
#define PEBIL_OPT_DFP 0x00000010
#define PEBIL_OPT_TRK 0x00000020
#define PEBIL_OPT_DOI 0x00000040

public:
    InstrumentationTool(ElfFile* elf);
    virtual ~InstrumentationTool() { }

    void init(char* ext);
    void initToolArgs(bool lpi, bool dtl, bool doi, uint32_t phase, char* inp, char* dfp, char* trk);

    virtual void declare();
    virtual void instrument();
    virtual void usesModifiedProgram() { }

    virtual const char* briefName() { __SHOULD_NOT_ARRIVE; }
    virtual const char* defaultExtension() { __SHOULD_NOT_ARRIVE; }
    const char* getExtension();
    bool verifyArgs();
    virtual uint32_t allowsArgs() { return PEBIL_OPT_ALL; }
    virtual uint32_t requiresArgs() { return PEBIL_OPT_NON; }
};


#endif /* _InstrumentationTool_h_ */
