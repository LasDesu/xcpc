#define MM_RD(addr,data) data=(*z80cpu->mm_rd)(z80cpu,addr)
#define MM_WR(addr,data) (*z80cpu->mm_wr)(z80cpu,addr,data)
#define IO_RD(addr,data) data=(*z80cpu->io_rd)(z80cpu,addr)
#define IO_WR(addr,data) (*z80cpu->io_wr)(z80cpu,addr,data)

#define M_CYCLES z80cpu->m_cycles
#define T_STATES z80cpu->t_states

#define AF_W z80cpu->AF.W
#define AF_H z80cpu->AF.B.h
#define AF_L z80cpu->AF.B.l
#define AF_P z80cpu->AF1.W
#define BC_W z80cpu->BC.W
#define BC_H z80cpu->BC.B.h
#define BC_L z80cpu->BC.B.l
#define BC_P z80cpu->BC1.W
#define DE_W z80cpu->DE.W
#define DE_H z80cpu->DE.B.h
#define DE_L z80cpu->DE.B.l
#define DE_P z80cpu->DE1.W
#define HL_W z80cpu->HL.W
#define HL_H z80cpu->HL.B.h
#define HL_L z80cpu->HL.B.l
#define HL_P z80cpu->HL1.W
#define IX_W z80cpu->IX.W
#define IX_H z80cpu->IX.B.h
#define IX_L z80cpu->IX.B.l
#define IY_W z80cpu->IY.W
#define IY_H z80cpu->IY.B.h
#define IY_L z80cpu->IY.B.l
#define SP_W z80cpu->SP.W
#define SP_H z80cpu->SP.B.h
#define SP_L z80cpu->SP.B.l
#define PC_W z80cpu->PC.W
#define PC_H z80cpu->PC.B.h
#define PC_L z80cpu->PC.B.l
#define IR_W z80cpu->IR.W
#define IR_H z80cpu->IR.B.h
#define IR_L z80cpu->IR.B.l
#define IF_W z80cpu->IF.W
#define IF_H z80cpu->IF.B.h
#define IF_L z80cpu->IF.B.l
#define WZ_W WZ.W
#define WZ_H WZ.B.h
#define WZ_L WZ.B.l

#define NOP_NULL_NULL(xxxx,yyyy,mc,ts) { \
  z80cpu->t_states-=ts; \
}

#define HLT_NULL_NULL(xxxx,yyyy,mc,ts) { \
  z80cpu->IF.W|=IFF_HALT;z80cpu->PC.W--; \
  z80cpu->t_states-=ts; \
}

#define MOV_RG08_RG08(reg1,reg2,mc,ts) { \
  reg1=reg2; \
  z80cpu->t_states-=ts; \
}

#define MOV_RG08_MM08(data,addr,mc,ts) { \
  data=(*z80cpu->mm_rd)(z80cpu,addr); \
  z80cpu->t_states-=ts; \
}

#define MOV_MM08_RG08(addr,data,mc,ts) { \
  (*z80cpu->mm_wr)(z80cpu,addr,data); \
  z80cpu->t_states-=ts; \
}

#define ADD_RG08_RG08(reg1,reg2,mc,ts) { \
  WZ.W=(I=reg1)+(J=reg2); reg1=WZ.B.l; \
  z80cpu->AF.B.l=(~(I^J)&(J^WZ.B.l)&0x80?V_FLAG:0)|WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define ADD_RG08_MM08(reg1,addr,mc,ts) { \
  WZ.W=(I=reg1)+(J=(*z80cpu->mm_rd)(z80cpu,addr)); reg1=WZ.B.l; \
  z80cpu->AF.B.l=(~(I^J)&(J^WZ.B.l)&0x80?V_FLAG:0)|WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define ADC_RG08_RG08(reg1,reg2,mc,ts) { \
  WZ.W=(I=reg1)+(J=reg2)+(z80cpu->AF.B.l&C_FLAG); reg1=WZ.B.l; \
  z80cpu->AF.B.l=(~(I^J)&(J^WZ.B.l)&0x80?V_FLAG:0)|WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define ADC_RG08_MM08(reg1,addr,mc,ts) { \
  WZ.W=(I=reg1)+(J=(*z80cpu->mm_rd)(z80cpu,addr))+(z80cpu->AF.B.l&C_FLAG); reg1=WZ.B.l; \
  z80cpu->AF.B.l=(~(I^J)&(J^WZ.B.l)&0x80?V_FLAG:0)|WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define SUB_RG08_RG08(reg1,reg2,mc,ts) { \
  WZ.W=(I=reg1)-(J=reg2); reg1=WZ.B.l; \
  z80cpu->AF.B.l=((I^J)&(I^WZ.B.l)&0x80?V_FLAG:0)|N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define SUB_RG08_MM08(reg1,addr,mc,ts) { \
  WZ.W=(I=reg1)-(J=(*z80cpu->mm_rd)(z80cpu,addr)); reg1=WZ.B.l; \
  z80cpu->AF.B.l=((I^J)&(I^WZ.B.l)&0x80?V_FLAG:0)|N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define SBC_RG08_RG08(reg1,reg2,mc,ts) { \
  WZ.W=(I=reg1)-(J=reg2)-(z80cpu->AF.B.l&C_FLAG); reg1=WZ.B.l; \
  z80cpu->AF.B.l=((I^J)&(I^WZ.B.l)&0x80?V_FLAG:0)|N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define SBC_RG08_MM08(reg1,addr,mc,ts) { \
  WZ.W=(I=reg1)-(J=(*z80cpu->mm_rd)(z80cpu,addr))-(z80cpu->AF.B.l&C_FLAG); reg1=WZ.B.l; \
  z80cpu->AF.B.l=((I^J)&(I^WZ.B.l)&0x80?V_FLAG:0)|N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define AND_RG08_RG08(reg1,reg2,mc,ts) { \
  reg1&=reg2;z80cpu->AF.B.l=PZSTable[reg1]|H_FLAG; \
  z80cpu->t_states-=ts; \
}

#define AND_RG08_MM08(reg1,addr,mc,ts) { \
  reg1&=(*z80cpu->mm_rd)(z80cpu,addr);z80cpu->AF.B.l=PZSTable[reg1]|H_FLAG; \
  z80cpu->t_states-=ts; \
}

#define XOR_RG08_RG08(reg1,reg2,mc,ts) { \
  reg1^=reg2;z80cpu->AF.B.l=PZSTable[reg1]; \
  z80cpu->t_states-=ts; \
}

#define XOR_RG08_MM08(reg1,addr,mc,ts) { \
  reg1^=(*z80cpu->mm_rd)(z80cpu,addr);z80cpu->AF.B.l=PZSTable[reg1]; \
  z80cpu->t_states-=ts; \
}

#define IOR_RG08_RG08(reg1,reg2,mc,ts) { \
  reg1|=reg2;z80cpu->AF.B.l=PZSTable[reg1]; \
  z80cpu->t_states-=ts; \
}

#define IOR_RG08_MM08(reg1,addr,mc,ts) { \
  reg1|=(*z80cpu->mm_rd)(z80cpu,addr);z80cpu->AF.B.l=PZSTable[reg1]; \
  z80cpu->t_states-=ts; \
}

#define CMP_RG08_RG08(reg1,reg2,mc,ts) { \
  WZ.W=(I=reg1)-(J=reg2); \
  z80cpu->AF.B.l=((I^J)&(I^WZ.B.l)&0x80?V_FLAG:0)|N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define CMP_RG08_MM08(reg1,addr,mc,ts) { \
  WZ.W=(I=reg1)-(J=(*z80cpu->mm_rd)(z80cpu,addr)); \
  z80cpu->AF.B.l=((I^J)&(I^WZ.B.l)&0x80?V_FLAG:0)|N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|((I^J^WZ.B.l)&H_FLAG); \
  z80cpu->t_states-=ts; \
}

#define RLC_RG08_NULL(dest,null,mc,ts) { \
  M_RLC(dest); \
  z80cpu->t_states-=ts; \
}

#define RLC_MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_RLC(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define RRC_RG08_NULL(dest,null,mc,ts) { \
  M_RRC(dest); \
  z80cpu->t_states-=ts; \
}

#define RRC_MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_RRC(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define RL__RG08_NULL(dest,null,mc,ts) { \
  M_RL(dest); \
  z80cpu->t_states-=ts; \
}

#define RL__MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_RL(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define RR__RG08_NULL(dest,null,mc,ts) { \
  M_RR(dest); \
  z80cpu->t_states-=ts; \
}

#define RR__MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_RR(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define SLA_RG08_NULL(dest,null,mc,ts) { \
  M_SLA(dest); \
  z80cpu->t_states-=ts; \
}

#define SLA_MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_SLA(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define SRA_RG08_NULL(dest,null,mc,ts) { \
  M_SRA(dest); \
  z80cpu->t_states-=ts; \
}

#define SRA_MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_SRA(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define SLL_RG08_NULL(dest,null,mc,ts) { \
  M_SLL(dest); \
  z80cpu->t_states-=ts; \
}

#define SLL_MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_SLL(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define SRL_RG08_NULL(dest,null,mc,ts) { \
  M_SRL(dest); \
  z80cpu->t_states-=ts; \
}

#define SRL_MM08_NULL(addr,null,mc,ts) { \
  I=(*z80cpu->mm_rd)(z80cpu,addr); M_SRL(I); (*z80cpu->mm_wr)(z80cpu,addr,I); \
  z80cpu->t_states-=ts; \
}

#define BIT_RG08_MASK(dest,mask,mc,ts) { \
  z80cpu->AF.B.l=(z80cpu->AF.B.l&C_FLAG)|H_FLAG|PZSTable[dest&mask]; \
  z80cpu->t_states-=ts; \
}

#define BIT_MM08_MASK(addr,mask,mc,ts) { \
  z80cpu->AF.B.l=(z80cpu->AF.B.l&C_FLAG)|H_FLAG|PZSTable[(*z80cpu->mm_rd)(z80cpu,addr)&mask]; \
  z80cpu->t_states-=ts; \
}

#define RES_RG08_MASK(dest,mask,mc,ts) { \
  dest&=~mask; \
  z80cpu->t_states-=ts; \
}

#define RES_MM08_MASK(addr,mask,mc,ts) { \
  (*z80cpu->mm_wr)(z80cpu,addr,((*z80cpu->mm_rd)(z80cpu,addr)&~mask)); \
  z80cpu->t_states-=ts; \
}

#define SET_RG08_MASK(dest,mask,mc,ts) { \
  dest|= mask; \
  z80cpu->t_states-=ts; \
}

#define SET_MM08_MASK(addr,mask,mc,ts) { \
  (*z80cpu->mm_wr)(z80cpu,addr,((*z80cpu->mm_rd)(z80cpu,addr)| mask)); \
  z80cpu->t_states-=ts; \
}

static guint8 Cycles[256] = {
   4,10, 7, 6, 4, 4, 7, 4, 4,11, 7, 6, 4, 4, 7, 4,
   8,10, 7, 6, 4, 4, 7, 4,12,11, 7, 6, 4, 4, 7, 4,
   7,10,16, 6, 4, 4, 7, 4, 7,11,16, 6, 4, 4, 7, 4,
   7,10,13, 6,11,11,10, 4, 7,11,13, 6, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   7, 7, 7, 7, 7, 7, 4, 7, 4, 4, 4, 4, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
   5,10,10,10,10,11, 7,11, 5,10,10, 0,10,17, 7,11,
   5,10,10,11,10,11, 7,11, 5, 4,10,11,10, 0, 7,11,
   5,10,10,19,10,11, 7,11, 5, 4,10, 4,10, 0, 7,11,
   5,10,10, 4,10,11, 7,11, 5, 6,10, 4,10, 0, 7,11 
};

static guint8 CyclesCB[256] = {
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
   8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8 
};

static guint8 CyclesED[256] = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  12,12,15,20, 8,14, 8, 9,12,12,15,20, 0,14, 0, 9,
  12,12,15,20, 0, 0, 8, 9,12,12,15,20, 0, 0, 8, 9,
  12,12,15,20, 0, 0, 0,18,12,12,15,20, 0, 0, 0,18,
  12, 0,15,20, 0, 0, 0, 0,12,12,15,20, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  16,16,16,16, 0, 0, 0, 0,16,16,16,16, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static guint8 CyclesXX[256] = {
   0, 0, 0, 0, 0, 0, 0, 0, 0,15, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0,15, 0, 0, 0, 0, 0, 0,
   0,14,20,10, 9, 9, 9, 0, 0,15,20,10, 9, 9, 9, 0,
   0, 0, 0, 0,23,23,19, 0, 0,15, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 9, 9,19, 0, 0, 0, 0, 0, 9, 9,19, 0,
   0, 0, 0, 0, 9, 9,19, 0, 0, 0, 0, 0, 9, 9,19, 0,
   9, 9, 9, 9, 9, 9,19, 9, 9, 9, 9, 9, 9, 9,19, 9,
  19,19,19,19,19,19,19,19, 0, 0, 0, 0, 9, 9,19, 0,
   0, 0, 0, 0, 9, 9,19, 0, 0, 0, 0, 0, 9, 9,19, 0,
   0, 0, 0, 0, 9, 9,19, 0, 0, 0, 0, 0, 9, 9,19, 0,
   0, 0, 0, 0, 9, 9,19, 0, 0, 0, 0, 0, 9, 9,19, 0,
   0, 0, 0, 0, 9, 9,19, 0, 0, 0, 0, 0, 9, 9,19, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0,14, 0,23, 0,15, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0
};

static guint8 CyclesXXCB[256] = {
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0,
   0, 0, 0, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0,23, 0
};

static guint8 ZSTable[256] = {
  Z_FLAG,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,
  S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG,S_FLAG
};

static guint8 PZSTable[256] = {
  Z_FLAG|P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
  0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
  0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
  0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
  P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
  0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
  P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
  P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
  0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG,S_FLAG|P_FLAG,S_FLAG|P_FLAG,S_FLAG,
  S_FLAG|P_FLAG,S_FLAG,S_FLAG,S_FLAG|P_FLAG
};

static guint16 DAATable[2048] = {
  0x0044,0x0100,0x0200,0x0304,0x0400,0x0504,0x0604,0x0700,
  0x0808,0x090C,0x1010,0x1114,0x1214,0x1310,0x1414,0x1510,
  0x1000,0x1104,0x1204,0x1300,0x1404,0x1500,0x1600,0x1704,
  0x180C,0x1908,0x2030,0x2134,0x2234,0x2330,0x2434,0x2530,
  0x2020,0x2124,0x2224,0x2320,0x2424,0x2520,0x2620,0x2724,
  0x282C,0x2928,0x3034,0x3130,0x3230,0x3334,0x3430,0x3534,
  0x3024,0x3120,0x3220,0x3324,0x3420,0x3524,0x3624,0x3720,
  0x3828,0x392C,0x4010,0x4114,0x4214,0x4310,0x4414,0x4510,
  0x4000,0x4104,0x4204,0x4300,0x4404,0x4500,0x4600,0x4704,
  0x480C,0x4908,0x5014,0x5110,0x5210,0x5314,0x5410,0x5514,
  0x5004,0x5100,0x5200,0x5304,0x5400,0x5504,0x5604,0x5700,
  0x5808,0x590C,0x6034,0x6130,0x6230,0x6334,0x6430,0x6534,
  0x6024,0x6120,0x6220,0x6324,0x6420,0x6524,0x6624,0x6720,
  0x6828,0x692C,0x7030,0x7134,0x7234,0x7330,0x7434,0x7530,
  0x7020,0x7124,0x7224,0x7320,0x7424,0x7520,0x7620,0x7724,
  0x782C,0x7928,0x8090,0x8194,0x8294,0x8390,0x8494,0x8590,
  0x8080,0x8184,0x8284,0x8380,0x8484,0x8580,0x8680,0x8784,
  0x888C,0x8988,0x9094,0x9190,0x9290,0x9394,0x9490,0x9594,
  0x9084,0x9180,0x9280,0x9384,0x9480,0x9584,0x9684,0x9780,
  0x9888,0x998C,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
  0x0045,0x0101,0x0201,0x0305,0x0401,0x0505,0x0605,0x0701,
  0x0809,0x090D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
  0x1001,0x1105,0x1205,0x1301,0x1405,0x1501,0x1601,0x1705,
  0x180D,0x1909,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
  0x2021,0x2125,0x2225,0x2321,0x2425,0x2521,0x2621,0x2725,
  0x282D,0x2929,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
  0x3025,0x3121,0x3221,0x3325,0x3421,0x3525,0x3625,0x3721,
  0x3829,0x392D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
  0x4001,0x4105,0x4205,0x4301,0x4405,0x4501,0x4601,0x4705,
  0x480D,0x4909,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
  0x5005,0x5101,0x5201,0x5305,0x5401,0x5505,0x5605,0x5701,
  0x5809,0x590D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
  0x6025,0x6121,0x6221,0x6325,0x6421,0x6525,0x6625,0x6721,
  0x6829,0x692D,0x7031,0x7135,0x7235,0x7331,0x7435,0x7531,
  0x7021,0x7125,0x7225,0x7321,0x7425,0x7521,0x7621,0x7725,
  0x782D,0x7929,0x8091,0x8195,0x8295,0x8391,0x8495,0x8591,
  0x8081,0x8185,0x8285,0x8381,0x8485,0x8581,0x8681,0x8785,
  0x888D,0x8989,0x9095,0x9191,0x9291,0x9395,0x9491,0x9595,
  0x9085,0x9181,0x9281,0x9385,0x9481,0x9585,0x9685,0x9781,
  0x9889,0x998D,0xA0B5,0xA1B1,0xA2B1,0xA3B5,0xA4B1,0xA5B5,
  0xA0A5,0xA1A1,0xA2A1,0xA3A5,0xA4A1,0xA5A5,0xA6A5,0xA7A1,
  0xA8A9,0xA9AD,0xB0B1,0xB1B5,0xB2B5,0xB3B1,0xB4B5,0xB5B1,
  0xB0A1,0xB1A5,0xB2A5,0xB3A1,0xB4A5,0xB5A1,0xB6A1,0xB7A5,
  0xB8AD,0xB9A9,0xC095,0xC191,0xC291,0xC395,0xC491,0xC595,
  0xC085,0xC181,0xC281,0xC385,0xC481,0xC585,0xC685,0xC781,
  0xC889,0xC98D,0xD091,0xD195,0xD295,0xD391,0xD495,0xD591,
  0xD081,0xD185,0xD285,0xD381,0xD485,0xD581,0xD681,0xD785,
  0xD88D,0xD989,0xE0B1,0xE1B5,0xE2B5,0xE3B1,0xE4B5,0xE5B1,
  0xE0A1,0xE1A5,0xE2A5,0xE3A1,0xE4A5,0xE5A1,0xE6A1,0xE7A5,
  0xE8AD,0xE9A9,0xF0B5,0xF1B1,0xF2B1,0xF3B5,0xF4B1,0xF5B5,
  0xF0A5,0xF1A1,0xF2A1,0xF3A5,0xF4A1,0xF5A5,0xF6A5,0xF7A1,
  0xF8A9,0xF9AD,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
  0x0045,0x0101,0x0201,0x0305,0x0401,0x0505,0x0605,0x0701,
  0x0809,0x090D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
  0x1001,0x1105,0x1205,0x1301,0x1405,0x1501,0x1601,0x1705,
  0x180D,0x1909,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
  0x2021,0x2125,0x2225,0x2321,0x2425,0x2521,0x2621,0x2725,
  0x282D,0x2929,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
  0x3025,0x3121,0x3221,0x3325,0x3421,0x3525,0x3625,0x3721,
  0x3829,0x392D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
  0x4001,0x4105,0x4205,0x4301,0x4405,0x4501,0x4601,0x4705,
  0x480D,0x4909,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
  0x5005,0x5101,0x5201,0x5305,0x5401,0x5505,0x5605,0x5701,
  0x5809,0x590D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
  0x0604,0x0700,0x0808,0x090C,0x0A0C,0x0B08,0x0C0C,0x0D08,
  0x0E08,0x0F0C,0x1010,0x1114,0x1214,0x1310,0x1414,0x1510,
  0x1600,0x1704,0x180C,0x1908,0x1A08,0x1B0C,0x1C08,0x1D0C,
  0x1E0C,0x1F08,0x2030,0x2134,0x2234,0x2330,0x2434,0x2530,
  0x2620,0x2724,0x282C,0x2928,0x2A28,0x2B2C,0x2C28,0x2D2C,
  0x2E2C,0x2F28,0x3034,0x3130,0x3230,0x3334,0x3430,0x3534,
  0x3624,0x3720,0x3828,0x392C,0x3A2C,0x3B28,0x3C2C,0x3D28,
  0x3E28,0x3F2C,0x4010,0x4114,0x4214,0x4310,0x4414,0x4510,
  0x4600,0x4704,0x480C,0x4908,0x4A08,0x4B0C,0x4C08,0x4D0C,
  0x4E0C,0x4F08,0x5014,0x5110,0x5210,0x5314,0x5410,0x5514,
  0x5604,0x5700,0x5808,0x590C,0x5A0C,0x5B08,0x5C0C,0x5D08,
  0x5E08,0x5F0C,0x6034,0x6130,0x6230,0x6334,0x6430,0x6534,
  0x6624,0x6720,0x6828,0x692C,0x6A2C,0x6B28,0x6C2C,0x6D28,
  0x6E28,0x6F2C,0x7030,0x7134,0x7234,0x7330,0x7434,0x7530,
  0x7620,0x7724,0x782C,0x7928,0x7A28,0x7B2C,0x7C28,0x7D2C,
  0x7E2C,0x7F28,0x8090,0x8194,0x8294,0x8390,0x8494,0x8590,
  0x8680,0x8784,0x888C,0x8988,0x8A88,0x8B8C,0x8C88,0x8D8C,
  0x8E8C,0x8F88,0x9094,0x9190,0x9290,0x9394,0x9490,0x9594,
  0x9684,0x9780,0x9888,0x998C,0x9A8C,0x9B88,0x9C8C,0x9D88,
  0x9E88,0x9F8C,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
  0x0605,0x0701,0x0809,0x090D,0x0A0D,0x0B09,0x0C0D,0x0D09,
  0x0E09,0x0F0D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
  0x1601,0x1705,0x180D,0x1909,0x1A09,0x1B0D,0x1C09,0x1D0D,
  0x1E0D,0x1F09,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
  0x2621,0x2725,0x282D,0x2929,0x2A29,0x2B2D,0x2C29,0x2D2D,
  0x2E2D,0x2F29,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
  0x3625,0x3721,0x3829,0x392D,0x3A2D,0x3B29,0x3C2D,0x3D29,
  0x3E29,0x3F2D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
  0x4601,0x4705,0x480D,0x4909,0x4A09,0x4B0D,0x4C09,0x4D0D,
  0x4E0D,0x4F09,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
  0x5605,0x5701,0x5809,0x590D,0x5A0D,0x5B09,0x5C0D,0x5D09,
  0x5E09,0x5F0D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
  0x6625,0x6721,0x6829,0x692D,0x6A2D,0x6B29,0x6C2D,0x6D29,
  0x6E29,0x6F2D,0x7031,0x7135,0x7235,0x7331,0x7435,0x7531,
  0x7621,0x7725,0x782D,0x7929,0x7A29,0x7B2D,0x7C29,0x7D2D,
  0x7E2D,0x7F29,0x8091,0x8195,0x8295,0x8391,0x8495,0x8591,
  0x8681,0x8785,0x888D,0x8989,0x8A89,0x8B8D,0x8C89,0x8D8D,
  0x8E8D,0x8F89,0x9095,0x9191,0x9291,0x9395,0x9491,0x9595,
  0x9685,0x9781,0x9889,0x998D,0x9A8D,0x9B89,0x9C8D,0x9D89,
  0x9E89,0x9F8D,0xA0B5,0xA1B1,0xA2B1,0xA3B5,0xA4B1,0xA5B5,
  0xA6A5,0xA7A1,0xA8A9,0xA9AD,0xAAAD,0xABA9,0xACAD,0xADA9,
  0xAEA9,0xAFAD,0xB0B1,0xB1B5,0xB2B5,0xB3B1,0xB4B5,0xB5B1,
  0xB6A1,0xB7A5,0xB8AD,0xB9A9,0xBAA9,0xBBAD,0xBCA9,0xBDAD,
  0xBEAD,0xBFA9,0xC095,0xC191,0xC291,0xC395,0xC491,0xC595,
  0xC685,0xC781,0xC889,0xC98D,0xCA8D,0xCB89,0xCC8D,0xCD89,
  0xCE89,0xCF8D,0xD091,0xD195,0xD295,0xD391,0xD495,0xD591,
  0xD681,0xD785,0xD88D,0xD989,0xDA89,0xDB8D,0xDC89,0xDD8D,
  0xDE8D,0xDF89,0xE0B1,0xE1B5,0xE2B5,0xE3B1,0xE4B5,0xE5B1,
  0xE6A1,0xE7A5,0xE8AD,0xE9A9,0xEAA9,0xEBAD,0xECA9,0xEDAD,
  0xEEAD,0xEFA9,0xF0B5,0xF1B1,0xF2B1,0xF3B5,0xF4B1,0xF5B5,
  0xF6A5,0xF7A1,0xF8A9,0xF9AD,0xFAAD,0xFBA9,0xFCAD,0xFDA9,
  0xFEA9,0xFFAD,0x0055,0x0111,0x0211,0x0315,0x0411,0x0515,
  0x0605,0x0701,0x0809,0x090D,0x0A0D,0x0B09,0x0C0D,0x0D09,
  0x0E09,0x0F0D,0x1011,0x1115,0x1215,0x1311,0x1415,0x1511,
  0x1601,0x1705,0x180D,0x1909,0x1A09,0x1B0D,0x1C09,0x1D0D,
  0x1E0D,0x1F09,0x2031,0x2135,0x2235,0x2331,0x2435,0x2531,
  0x2621,0x2725,0x282D,0x2929,0x2A29,0x2B2D,0x2C29,0x2D2D,
  0x2E2D,0x2F29,0x3035,0x3131,0x3231,0x3335,0x3431,0x3535,
  0x3625,0x3721,0x3829,0x392D,0x3A2D,0x3B29,0x3C2D,0x3D29,
  0x3E29,0x3F2D,0x4011,0x4115,0x4215,0x4311,0x4415,0x4511,
  0x4601,0x4705,0x480D,0x4909,0x4A09,0x4B0D,0x4C09,0x4D0D,
  0x4E0D,0x4F09,0x5015,0x5111,0x5211,0x5315,0x5411,0x5515,
  0x5605,0x5701,0x5809,0x590D,0x5A0D,0x5B09,0x5C0D,0x5D09,
  0x5E09,0x5F0D,0x6035,0x6131,0x6231,0x6335,0x6431,0x6535,
  0x0046,0x0102,0x0202,0x0306,0x0402,0x0506,0x0606,0x0702,
  0x080A,0x090E,0x0402,0x0506,0x0606,0x0702,0x080A,0x090E,
  0x1002,0x1106,0x1206,0x1302,0x1406,0x1502,0x1602,0x1706,
  0x180E,0x190A,0x1406,0x1502,0x1602,0x1706,0x180E,0x190A,
  0x2022,0x2126,0x2226,0x2322,0x2426,0x2522,0x2622,0x2726,
  0x282E,0x292A,0x2426,0x2522,0x2622,0x2726,0x282E,0x292A,
  0x3026,0x3122,0x3222,0x3326,0x3422,0x3526,0x3626,0x3722,
  0x382A,0x392E,0x3422,0x3526,0x3626,0x3722,0x382A,0x392E,
  0x4002,0x4106,0x4206,0x4302,0x4406,0x4502,0x4602,0x4706,
  0x480E,0x490A,0x4406,0x4502,0x4602,0x4706,0x480E,0x490A,
  0x5006,0x5102,0x5202,0x5306,0x5402,0x5506,0x5606,0x5702,
  0x580A,0x590E,0x5402,0x5506,0x5606,0x5702,0x580A,0x590E,
  0x6026,0x6122,0x6222,0x6326,0x6422,0x6526,0x6626,0x6722,
  0x682A,0x692E,0x6422,0x6526,0x6626,0x6722,0x682A,0x692E,
  0x7022,0x7126,0x7226,0x7322,0x7426,0x7522,0x7622,0x7726,
  0x782E,0x792A,0x7426,0x7522,0x7622,0x7726,0x782E,0x792A,
  0x8082,0x8186,0x8286,0x8382,0x8486,0x8582,0x8682,0x8786,
  0x888E,0x898A,0x8486,0x8582,0x8682,0x8786,0x888E,0x898A,
  0x9086,0x9182,0x9282,0x9386,0x9482,0x9586,0x9686,0x9782,
  0x988A,0x998E,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
  0x4003,0x4107,0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,
  0x480F,0x490B,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
  0x5007,0x5103,0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,
  0x580B,0x590F,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
  0x6027,0x6123,0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,
  0x682B,0x692F,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
  0x7023,0x7127,0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,
  0x782F,0x792B,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
  0x8083,0x8187,0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,
  0x888F,0x898B,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
  0x9087,0x9183,0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,
  0x988B,0x998F,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F,
  0xA0A7,0xA1A3,0xA2A3,0xA3A7,0xA4A3,0xA5A7,0xA6A7,0xA7A3,
  0xA8AB,0xA9AF,0xA4A3,0xA5A7,0xA6A7,0xA7A3,0xA8AB,0xA9AF,
  0xB0A3,0xB1A7,0xB2A7,0xB3A3,0xB4A7,0xB5A3,0xB6A3,0xB7A7,
  0xB8AF,0xB9AB,0xB4A7,0xB5A3,0xB6A3,0xB7A7,0xB8AF,0xB9AB,
  0xC087,0xC183,0xC283,0xC387,0xC483,0xC587,0xC687,0xC783,
  0xC88B,0xC98F,0xC483,0xC587,0xC687,0xC783,0xC88B,0xC98F,
  0xD083,0xD187,0xD287,0xD383,0xD487,0xD583,0xD683,0xD787,
  0xD88F,0xD98B,0xD487,0xD583,0xD683,0xD787,0xD88F,0xD98B,
  0xE0A3,0xE1A7,0xE2A7,0xE3A3,0xE4A7,0xE5A3,0xE6A3,0xE7A7,
  0xE8AF,0xE9AB,0xE4A7,0xE5A3,0xE6A3,0xE7A7,0xE8AF,0xE9AB,
  0xF0A7,0xF1A3,0xF2A3,0xF3A7,0xF4A3,0xF5A7,0xF6A7,0xF7A3,
  0xF8AB,0xF9AF,0xF4A3,0xF5A7,0xF6A7,0xF7A3,0xF8AB,0xF9AF,
  0x0047,0x0103,0x0203,0x0307,0x0403,0x0507,0x0607,0x0703,
  0x080B,0x090F,0x0403,0x0507,0x0607,0x0703,0x080B,0x090F,
  0x1003,0x1107,0x1207,0x1303,0x1407,0x1503,0x1603,0x1707,
  0x180F,0x190B,0x1407,0x1503,0x1603,0x1707,0x180F,0x190B,
  0x2023,0x2127,0x2227,0x2323,0x2427,0x2523,0x2623,0x2727,
  0x282F,0x292B,0x2427,0x2523,0x2623,0x2727,0x282F,0x292B,
  0x3027,0x3123,0x3223,0x3327,0x3423,0x3527,0x3627,0x3723,
  0x382B,0x392F,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
  0x4003,0x4107,0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,
  0x480F,0x490B,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
  0x5007,0x5103,0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,
  0x580B,0x590F,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
  0x6027,0x6123,0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,
  0x682B,0x692F,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
  0x7023,0x7127,0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,
  0x782F,0x792B,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
  0x8083,0x8187,0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,
  0x888F,0x898B,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
  0x9087,0x9183,0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,
  0x988B,0x998F,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F,
  0xFABE,0xFBBA,0xFCBE,0xFDBA,0xFEBA,0xFFBE,0x0046,0x0102,
  0x0202,0x0306,0x0402,0x0506,0x0606,0x0702,0x080A,0x090E,
  0x0A1E,0x0B1A,0x0C1E,0x0D1A,0x0E1A,0x0F1E,0x1002,0x1106,
  0x1206,0x1302,0x1406,0x1502,0x1602,0x1706,0x180E,0x190A,
  0x1A1A,0x1B1E,0x1C1A,0x1D1E,0x1E1E,0x1F1A,0x2022,0x2126,
  0x2226,0x2322,0x2426,0x2522,0x2622,0x2726,0x282E,0x292A,
  0x2A3A,0x2B3E,0x2C3A,0x2D3E,0x2E3E,0x2F3A,0x3026,0x3122,
  0x3222,0x3326,0x3422,0x3526,0x3626,0x3722,0x382A,0x392E,
  0x3A3E,0x3B3A,0x3C3E,0x3D3A,0x3E3A,0x3F3E,0x4002,0x4106,
  0x4206,0x4302,0x4406,0x4502,0x4602,0x4706,0x480E,0x490A,
  0x4A1A,0x4B1E,0x4C1A,0x4D1E,0x4E1E,0x4F1A,0x5006,0x5102,
  0x5202,0x5306,0x5402,0x5506,0x5606,0x5702,0x580A,0x590E,
  0x5A1E,0x5B1A,0x5C1E,0x5D1A,0x5E1A,0x5F1E,0x6026,0x6122,
  0x6222,0x6326,0x6422,0x6526,0x6626,0x6722,0x682A,0x692E,
  0x6A3E,0x6B3A,0x6C3E,0x6D3A,0x6E3A,0x6F3E,0x7022,0x7126,
  0x7226,0x7322,0x7426,0x7522,0x7622,0x7726,0x782E,0x792A,
  0x7A3A,0x7B3E,0x7C3A,0x7D3E,0x7E3E,0x7F3A,0x8082,0x8186,
  0x8286,0x8382,0x8486,0x8582,0x8682,0x8786,0x888E,0x898A,
  0x8A9A,0x8B9E,0x8C9A,0x8D9E,0x8E9E,0x8F9A,0x9086,0x9182,
  0x9282,0x9386,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
  0x3A3F,0x3B3B,0x3C3F,0x3D3B,0x3E3B,0x3F3F,0x4003,0x4107,
  0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
  0x4A1B,0x4B1F,0x4C1B,0x4D1F,0x4E1F,0x4F1B,0x5007,0x5103,
  0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
  0x5A1F,0x5B1B,0x5C1F,0x5D1B,0x5E1B,0x5F1F,0x6027,0x6123,
  0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
  0x6A3F,0x6B3B,0x6C3F,0x6D3B,0x6E3B,0x6F3F,0x7023,0x7127,
  0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
  0x7A3B,0x7B3F,0x7C3B,0x7D3F,0x7E3F,0x7F3B,0x8083,0x8187,
  0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
  0x8A9B,0x8B9F,0x8C9B,0x8D9F,0x8E9F,0x8F9B,0x9087,0x9183,
  0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F,
  0x9A9F,0x9B9B,0x9C9F,0x9D9B,0x9E9B,0x9F9F,0xA0A7,0xA1A3,
  0xA2A3,0xA3A7,0xA4A3,0xA5A7,0xA6A7,0xA7A3,0xA8AB,0xA9AF,
  0xAABF,0xABBB,0xACBF,0xADBB,0xAEBB,0xAFBF,0xB0A3,0xB1A7,
  0xB2A7,0xB3A3,0xB4A7,0xB5A3,0xB6A3,0xB7A7,0xB8AF,0xB9AB,
  0xBABB,0xBBBF,0xBCBB,0xBDBF,0xBEBF,0xBFBB,0xC087,0xC183,
  0xC283,0xC387,0xC483,0xC587,0xC687,0xC783,0xC88B,0xC98F,
  0xCA9F,0xCB9B,0xCC9F,0xCD9B,0xCE9B,0xCF9F,0xD083,0xD187,
  0xD287,0xD383,0xD487,0xD583,0xD683,0xD787,0xD88F,0xD98B,
  0xDA9B,0xDB9F,0xDC9B,0xDD9F,0xDE9F,0xDF9B,0xE0A3,0xE1A7,
  0xE2A7,0xE3A3,0xE4A7,0xE5A3,0xE6A3,0xE7A7,0xE8AF,0xE9AB,
  0xEABB,0xEBBF,0xECBB,0xEDBF,0xEEBF,0xEFBB,0xF0A7,0xF1A3,
  0xF2A3,0xF3A7,0xF4A3,0xF5A7,0xF6A7,0xF7A3,0xF8AB,0xF9AF,
  0xFABF,0xFBBB,0xFCBF,0xFDBB,0xFEBB,0xFFBF,0x0047,0x0103,
  0x0203,0x0307,0x0403,0x0507,0x0607,0x0703,0x080B,0x090F,
  0x0A1F,0x0B1B,0x0C1F,0x0D1B,0x0E1B,0x0F1F,0x1003,0x1107,
  0x1207,0x1303,0x1407,0x1503,0x1603,0x1707,0x180F,0x190B,
  0x1A1B,0x1B1F,0x1C1B,0x1D1F,0x1E1F,0x1F1B,0x2023,0x2127,
  0x2227,0x2323,0x2427,0x2523,0x2623,0x2727,0x282F,0x292B,
  0x2A3B,0x2B3F,0x2C3B,0x2D3F,0x2E3F,0x2F3B,0x3027,0x3123,
  0x3223,0x3327,0x3423,0x3527,0x3627,0x3723,0x382B,0x392F,
  0x3A3F,0x3B3B,0x3C3F,0x3D3B,0x3E3B,0x3F3F,0x4003,0x4107,
  0x4207,0x4303,0x4407,0x4503,0x4603,0x4707,0x480F,0x490B,
  0x4A1B,0x4B1F,0x4C1B,0x4D1F,0x4E1F,0x4F1B,0x5007,0x5103,
  0x5203,0x5307,0x5403,0x5507,0x5607,0x5703,0x580B,0x590F,
  0x5A1F,0x5B1B,0x5C1F,0x5D1B,0x5E1B,0x5F1F,0x6027,0x6123,
  0x6223,0x6327,0x6423,0x6527,0x6627,0x6723,0x682B,0x692F,
  0x6A3F,0x6B3B,0x6C3F,0x6D3B,0x6E3B,0x6F3F,0x7023,0x7127,
  0x7227,0x7323,0x7427,0x7523,0x7623,0x7727,0x782F,0x792B,
  0x7A3B,0x7B3F,0x7C3B,0x7D3F,0x7E3F,0x7F3B,0x8083,0x8187,
  0x8287,0x8383,0x8487,0x8583,0x8683,0x8787,0x888F,0x898B,
  0x8A9B,0x8B9F,0x8C9B,0x8D9F,0x8E9F,0x8F9B,0x9087,0x9183,
  0x9283,0x9387,0x9483,0x9587,0x9687,0x9783,0x988B,0x998F
};

#define WrZ80(addr,value)  ((*z80cpu->mm_wr)((z80cpu),(addr),(value)))
#define RdZ80(addr)        ((*z80cpu->mm_rd)((z80cpu),(addr)))
#define OutZ80(addr,value) ((*z80cpu->io_wr)((z80cpu),(addr),(value)))
#define InZ80(addr)        ((*z80cpu->io_rd)((z80cpu),(addr)))

#define S(Fl)        z80cpu->AF.B.l|=Fl
#define R(Fl)        z80cpu->AF.B.l&=~(Fl)
#define FLAGS(Rg,Fl) z80cpu->AF.B.l=Fl|ZSTable[Rg]

#define M_RLC(Rg)      \
  z80cpu->AF.B.l=Rg>>7;Rg=(Rg<<1)|z80cpu->AF.B.l;z80cpu->AF.B.l|=PZSTable[Rg]
#define M_RRC(Rg)      \
  z80cpu->AF.B.l=Rg&0x01;Rg=(Rg>>1)|(z80cpu->AF.B.l<<7);z80cpu->AF.B.l|=PZSTable[Rg]
#define M_RL(Rg)       \
  if(Rg&0x80)          \
  {                    \
    Rg=(Rg<<1)|(z80cpu->AF.B.l&C_FLAG); \
    z80cpu->AF.B.l=PZSTable[Rg]|C_FLAG; \
  }                    \
  else                 \
  {                    \
    Rg=(Rg<<1)|(z80cpu->AF.B.l&C_FLAG); \
    z80cpu->AF.B.l=PZSTable[Rg];        \
  }
#define M_RR(Rg)       \
  if(Rg&0x01)          \
  {                    \
    Rg=(Rg>>1)|(z80cpu->AF.B.l<<7);     \
    z80cpu->AF.B.l=PZSTable[Rg]|C_FLAG; \
  }                    \
  else                 \
  {                    \
    Rg=(Rg>>1)|(z80cpu->AF.B.l<<7);     \
    z80cpu->AF.B.l=PZSTable[Rg];        \
  }

#define M_SLA(Rg)      \
  z80cpu->AF.B.l=Rg>>7;Rg<<=1;z80cpu->AF.B.l|=PZSTable[Rg]
#define M_SRA(Rg)      \
  z80cpu->AF.B.l=Rg&C_FLAG;Rg=(Rg>>1)|(Rg&0x80);z80cpu->AF.B.l|=PZSTable[Rg]

#define M_SLL(Rg)      \
  z80cpu->AF.B.l=Rg>>7;Rg=(Rg<<1)|0x01;z80cpu->AF.B.l|=PZSTable[Rg]
#define M_SRL(Rg)      \
  z80cpu->AF.B.l=Rg&0x01;Rg>>=1;z80cpu->AF.B.l|=PZSTable[Rg]

#define M_BIT(Bit,Rg)  \
  z80cpu->AF.B.l=(z80cpu->AF.B.l&C_FLAG)|H_FLAG|PZSTable[Rg&(1<<Bit)]

#define M_SET(Bit,Rg) Rg|=1<<Bit
#define M_RES(Bit,Rg) Rg&=~(1<<Bit)

#define M_POP(Rg)      \
  z80cpu->Rg.B.l=RdZ80(z80cpu->SP.W++);z80cpu->Rg.B.h=RdZ80(z80cpu->SP.W++)
#define M_PUSH(Rg)     \
  WrZ80(--z80cpu->SP.W,z80cpu->Rg.B.h);WrZ80(--z80cpu->SP.W,z80cpu->Rg.B.l)

#define M_CALL         \
  WZ.B.l=RdZ80(z80cpu->PC.W++);WZ.B.h=RdZ80(z80cpu->PC.W++);         \
  WrZ80(--z80cpu->SP.W,z80cpu->PC.B.h);WrZ80(--z80cpu->SP.W,z80cpu->PC.B.l); \
  z80cpu->PC.W=WZ.W

#define M_JP  WZ.B.l=RdZ80(z80cpu->PC.W++);WZ.B.h=RdZ80(z80cpu->PC.W);z80cpu->PC.W=WZ.W
#define M_JR  z80cpu->PC.W+=(gint8)RdZ80(z80cpu->PC.W)+1
#define M_RET z80cpu->PC.B.l=RdZ80(z80cpu->SP.W++);z80cpu->PC.B.h=RdZ80(z80cpu->SP.W++)

#define M_RST(Ad)      \
  WrZ80(--z80cpu->SP.W,z80cpu->PC.B.h);WrZ80(--z80cpu->SP.W,z80cpu->PC.B.l);z80cpu->PC.W=Ad

#define M_LDWORD(Rg)   \
  z80cpu->Rg.B.l=RdZ80(z80cpu->PC.W++);z80cpu->Rg.B.h=RdZ80(z80cpu->PC.W++)

#define M_ADD(Rg)      \
  WZ.W=z80cpu->AF.B.h+Rg;     \
  z80cpu->AF.B.l=            \
    (~(z80cpu->AF.B.h^Rg)&(Rg^WZ.B.l)&0x80? V_FLAG:0)| \
    WZ.B.h|ZSTable[WZ.B.l]|                        \
    ((z80cpu->AF.B.h^Rg^WZ.B.l)&H_FLAG);               \
  z80cpu->AF.B.h=WZ.B.l       

#define M_SUB(Rg)      \
  WZ.W=z80cpu->AF.B.h-Rg;    \
  z80cpu->AF.B.l=           \
    ((z80cpu->AF.B.h^Rg)&(z80cpu->AF.B.h^WZ.B.l)&0x80? V_FLAG:0)| \
    N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|                      \
    ((z80cpu->AF.B.h^Rg^WZ.B.l)&H_FLAG);                     \
  z80cpu->AF.B.h=WZ.B.l

#define M_ADC(Rg)      \
  WZ.W=z80cpu->AF.B.h+Rg+(z80cpu->AF.B.l&C_FLAG); \
  z80cpu->AF.B.l=                           \
    (~(z80cpu->AF.B.h^Rg)&(Rg^WZ.B.l)&0x80? V_FLAG:0)| \
    WZ.B.h|ZSTable[WZ.B.l]|              \
    ((z80cpu->AF.B.h^Rg^WZ.B.l)&H_FLAG);     \
  z80cpu->AF.B.h=WZ.B.l

#define M_SBC(Rg)      \
  WZ.W=z80cpu->AF.B.h-Rg-(z80cpu->AF.B.l&C_FLAG); \
  z80cpu->AF.B.l=                           \
    ((z80cpu->AF.B.h^Rg)&(z80cpu->AF.B.h^WZ.B.l)&0x80? V_FLAG:0)| \
    N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|      \
    ((z80cpu->AF.B.h^Rg^WZ.B.l)&H_FLAG);     \
  z80cpu->AF.B.h=WZ.B.l

#define M_CMP(Rg)       \
  WZ.W=z80cpu->AF.B.h-Rg;    \
  z80cpu->AF.B.l=           \
    ((z80cpu->AF.B.h^Rg)&(z80cpu->AF.B.h^WZ.B.l)&0x80? V_FLAG:0)| \
    N_FLAG|-WZ.B.h|ZSTable[WZ.B.l]|                      \
    ((z80cpu->AF.B.h^Rg^WZ.B.l)&H_FLAG)

#define M_AND(Rg) z80cpu->AF.B.h&=Rg;z80cpu->AF.B.l=PZSTable[z80cpu->AF.B.h]|H_FLAG
#define M_IOR(Rg) z80cpu->AF.B.h|=Rg;z80cpu->AF.B.l=PZSTable[z80cpu->AF.B.h]
#define M_XOR(Rg) z80cpu->AF.B.h^=Rg;z80cpu->AF.B.l=PZSTable[z80cpu->AF.B.h]

#define M_IN(Rg)        \
  Rg=InZ80(z80cpu->BC.W);  \
  z80cpu->AF.B.l=PZSTable[Rg]|(z80cpu->AF.B.l&C_FLAG)

#define M_INC(Rg)       \
  Rg++;                 \
  z80cpu->AF.B.l=            \
    (z80cpu->AF.B.l&C_FLAG)|ZSTable[Rg]|           \
    (Rg==0x80? V_FLAG:0)|(Rg&0x0F? 0:H_FLAG)

#define M_DEC(Rg)       \
  Rg--;                 \
  z80cpu->AF.B.l=            \
    N_FLAG|(z80cpu->AF.B.l&C_FLAG)|ZSTable[Rg]| \
    (Rg==0x7F? V_FLAG:0)|((Rg&0x0F)==0x0F? H_FLAG:0)

#define M_ADDW(Rg1,Rg2) \
  WZ.W=(z80cpu->Rg1.W+z80cpu->Rg2.W)&0xFFFF;                        \
  z80cpu->AF.B.l=                                             \
    (z80cpu->AF.B.l&~(H_FLAG|N_FLAG|C_FLAG))|                 \
    ((z80cpu->Rg1.W^z80cpu->Rg2.W^WZ.W)&0x1000? H_FLAG:0)|          \
    (((long)z80cpu->Rg1.W+(long)z80cpu->Rg2.W)&0x10000? C_FLAG:0); \
  z80cpu->Rg1.W=WZ.W

#define M_ADCW(Rg)      \
  I=z80cpu->AF.B.l&C_FLAG;WZ.W=(z80cpu->HL.W+z80cpu->Rg.W+I)&0xFFFF;           \
  z80cpu->AF.B.l=                                                   \
    (((long)z80cpu->HL.W+(long)z80cpu->Rg.W+(long)I)&0x10000? C_FLAG:0)| \
    (~(z80cpu->HL.W^z80cpu->Rg.W)&(z80cpu->Rg.W^WZ.W)&0x8000? V_FLAG:0)|       \
    ((z80cpu->HL.W^z80cpu->Rg.W^WZ.W)&0x1000? H_FLAG:0)|                  \
    (WZ.W? 0:Z_FLAG)|(WZ.B.h&S_FLAG);                            \
  z80cpu->HL.W=WZ.W

#define M_SBCW(Rg)      \
  I=z80cpu->AF.B.l&C_FLAG;WZ.W=(z80cpu->HL.W-z80cpu->Rg.W-I)&0xFFFF;           \
  z80cpu->AF.B.l=                                                   \
    N_FLAG|                                                    \
    (((long)z80cpu->HL.W-(long)z80cpu->Rg.W-(long)I)&0x10000? C_FLAG:0)| \
    ((z80cpu->HL.W^z80cpu->Rg.W)&(z80cpu->HL.W^WZ.W)&0x8000? V_FLAG:0)|        \
    ((z80cpu->HL.W^z80cpu->Rg.W^WZ.W)&0x1000? H_FLAG:0)|                  \
    (WZ.W? 0:Z_FLAG)|(WZ.B.h&S_FLAG);                            \
  z80cpu->HL.W=WZ.W

enum Codes {
  NOP,LD_BC_WORD,LD_xBC_A,INC_BC,INC_B,DEC_B,LD_B_BYTE,RLCA,
  EX_AF_AF,ADD_HL_BC,LD_A_xBC,DEC_BC,INC_C,DEC_C,LD_C_BYTE,RRCA,
  DJNZ,LD_DE_WORD,LD_xDE_A,INC_DE,INC_D,DEC_D,LD_D_BYTE,RLA,
  JR,ADD_HL_DE,LD_A_xDE,DEC_DE,INC_E,DEC_E,LD_E_BYTE,RRA,
  JR_NZ,LD_HL_WORD,LD_xWORD_HL,INC_HL,INC_H,DEC_H,LD_H_BYTE,DAA,
  JR_Z,ADD_HL_HL,LD_HL_xWORD,DEC_HL,INC_L,DEC_L,LD_L_BYTE,CPL,
  JR_NC,LD_SP_WORD,LD_xWORD_A,INC_SP,INC_xHL,DEC_xHL,LD_xHL_BYTE,SCF,
  JR_C,ADD_HL_SP,LD_A_xWORD,DEC_SP,INC_A,DEC_A,LD_A_BYTE,CCF,
  LD_B_B,LD_B_C,LD_B_D,LD_B_E,LD_B_H,LD_B_L,LD_B_xHL,LD_B_A,
  LD_C_B,LD_C_C,LD_C_D,LD_C_E,LD_C_H,LD_C_L,LD_C_xHL,LD_C_A,
  LD_D_B,LD_D_C,LD_D_D,LD_D_E,LD_D_H,LD_D_L,LD_D_xHL,LD_D_A,
  LD_E_B,LD_E_C,LD_E_D,LD_E_E,LD_E_H,LD_E_L,LD_E_xHL,LD_E_A,
  LD_H_B,LD_H_C,LD_H_D,LD_H_E,LD_H_H,LD_H_L,LD_H_xHL,LD_H_A,
  LD_L_B,LD_L_C,LD_L_D,LD_L_E,LD_L_H,LD_L_L,LD_L_xHL,LD_L_A,
  LD_xHL_B,LD_xHL_C,LD_xHL_D,LD_xHL_E,LD_xHL_H,LD_xHL_L,HALT,LD_xHL_A,
  LD_A_B,LD_A_C,LD_A_D,LD_A_E,LD_A_H,LD_A_L,LD_A_xHL,LD_A_A,
  ADD_B,ADD_C,ADD_D,ADD_E,ADD_H,ADD_L,ADD_xHL,ADD_A,
  ADC_B,ADC_C,ADC_D,ADC_E,ADC_H,ADC_L,ADC_xHL,ADC_A,
  SUB_B,SUB_C,SUB_D,SUB_E,SUB_H,SUB_L,SUB_xHL,SUB_A,
  SBC_B,SBC_C,SBC_D,SBC_E,SBC_H,SBC_L,SBC_xHL,SBC_A,
  AND_B,AND_C,AND_D,AND_E,AND_H,AND_L,AND_xHL,AND_A,
  XOR_B,XOR_C,XOR_D,XOR_E,XOR_H,XOR_L,XOR_xHL,XOR_A,
  OR_B,OR_C,OR_D,OR_E,OR_H,OR_L,OR_xHL,OR_A,
  CP_B,CP_C,CP_D,CP_E,CP_H,CP_L,CP_xHL,CP_A,
  RET_NZ,POP_BC,JP_NZ,JP,CALL_NZ,PUSH_BC,ADD_BYTE,RST00,
  RET_Z,RET,JP_Z,PFX_CB,CALL_Z,CALL,ADC_BYTE,RST08,
  RET_NC,POP_DE,JP_NC,OUTA,CALL_NC,PUSH_DE,SUB_BYTE,RST10,
  RET_C,EXX,JP_C,INA,CALL_C,PFX_DD,SBC_BYTE,RST18,
  RET_PO,POP_HL,JP_PO,EX_HL_xSP,CALL_PO,PUSH_HL,AND_BYTE,RST20,
  RET_PE,LD_PC_HL,JP_PE,EX_DE_HL,CALL_PE,PFX_ED,XOR_BYTE,RST28,
  RET_P,POP_AF,JP_P,DI,CALL_P,PUSH_AF,OR_BYTE,RST30,
  RET_M,LD_SP_HL,JP_M,EI,CALL_M,PFX_FD,CP_BYTE,RST38
};

enum CodesCB {
  RLC_B,RLC_C,RLC_D,RLC_E,RLC_H,RLC_L,RLC_xHL,RLC_A,
  RRC_B,RRC_C,RRC_D,RRC_E,RRC_H,RRC_L,RRC_xHL,RRC_A,
  RL_B,RL_C,RL_D,RL_E,RL_H,RL_L,RL_xHL,RL_A,
  RR_B,RR_C,RR_D,RR_E,RR_H,RR_L,RR_xHL,RR_A,
  SLA_B,SLA_C,SLA_D,SLA_E,SLA_H,SLA_L,SLA_xHL,SLA_A,
  SRA_B,SRA_C,SRA_D,SRA_E,SRA_H,SRA_L,SRA_xHL,SRA_A,
  SLL_B,SLL_C,SLL_D,SLL_E,SLL_H,SLL_L,SLL_xHL,SLL_A,
  SRL_B,SRL_C,SRL_D,SRL_E,SRL_H,SRL_L,SRL_xHL,SRL_A,
  BIT0_B,BIT0_C,BIT0_D,BIT0_E,BIT0_H,BIT0_L,BIT0_xHL,BIT0_A,
  BIT1_B,BIT1_C,BIT1_D,BIT1_E,BIT1_H,BIT1_L,BIT1_xHL,BIT1_A,
  BIT2_B,BIT2_C,BIT2_D,BIT2_E,BIT2_H,BIT2_L,BIT2_xHL,BIT2_A,
  BIT3_B,BIT3_C,BIT3_D,BIT3_E,BIT3_H,BIT3_L,BIT3_xHL,BIT3_A,
  BIT4_B,BIT4_C,BIT4_D,BIT4_E,BIT4_H,BIT4_L,BIT4_xHL,BIT4_A,
  BIT5_B,BIT5_C,BIT5_D,BIT5_E,BIT5_H,BIT5_L,BIT5_xHL,BIT5_A,
  BIT6_B,BIT6_C,BIT6_D,BIT6_E,BIT6_H,BIT6_L,BIT6_xHL,BIT6_A,
  BIT7_B,BIT7_C,BIT7_D,BIT7_E,BIT7_H,BIT7_L,BIT7_xHL,BIT7_A,
  RES0_B,RES0_C,RES0_D,RES0_E,RES0_H,RES0_L,RES0_xHL,RES0_A,
  RES1_B,RES1_C,RES1_D,RES1_E,RES1_H,RES1_L,RES1_xHL,RES1_A,
  RES2_B,RES2_C,RES2_D,RES2_E,RES2_H,RES2_L,RES2_xHL,RES2_A,
  RES3_B,RES3_C,RES3_D,RES3_E,RES3_H,RES3_L,RES3_xHL,RES3_A,
  RES4_B,RES4_C,RES4_D,RES4_E,RES4_H,RES4_L,RES4_xHL,RES4_A,
  RES5_B,RES5_C,RES5_D,RES5_E,RES5_H,RES5_L,RES5_xHL,RES5_A,
  RES6_B,RES6_C,RES6_D,RES6_E,RES6_H,RES6_L,RES6_xHL,RES6_A,
  RES7_B,RES7_C,RES7_D,RES7_E,RES7_H,RES7_L,RES7_xHL,RES7_A,  
  SET0_B,SET0_C,SET0_D,SET0_E,SET0_H,SET0_L,SET0_xHL,SET0_A,
  SET1_B,SET1_C,SET1_D,SET1_E,SET1_H,SET1_L,SET1_xHL,SET1_A,
  SET2_B,SET2_C,SET2_D,SET2_E,SET2_H,SET2_L,SET2_xHL,SET2_A,
  SET3_B,SET3_C,SET3_D,SET3_E,SET3_H,SET3_L,SET3_xHL,SET3_A,
  SET4_B,SET4_C,SET4_D,SET4_E,SET4_H,SET4_L,SET4_xHL,SET4_A,
  SET5_B,SET5_C,SET5_D,SET5_E,SET5_H,SET5_L,SET5_xHL,SET5_A,
  SET6_B,SET6_C,SET6_D,SET6_E,SET6_H,SET6_L,SET6_xHL,SET6_A,
  SET7_B,SET7_C,SET7_D,SET7_E,SET7_H,SET7_L,SET7_xHL,SET7_A
};

enum CodesED {
  DB_00,DB_01,DB_02,DB_03,DB_04,DB_05,DB_06,DB_07,
  DB_08,DB_09,DB_0A,DB_0B,DB_0C,DB_0D,DB_0E,DB_0F,
  DB_10,DB_11,DB_12,DB_13,DB_14,DB_15,DB_16,DB_17,
  DB_18,DB_19,DB_1A,DB_1B,DB_1C,DB_1D,DB_1E,DB_1F,
  DB_20,DB_21,DB_22,DB_23,DB_24,DB_25,DB_26,DB_27,
  DB_28,DB_29,DB_2A,DB_2B,DB_2C,DB_2D,DB_2E,DB_2F,
  DB_30,DB_31,DB_32,DB_33,DB_34,DB_35,DB_36,DB_37,
  DB_38,DB_39,DB_3A,DB_3B,DB_3C,DB_3D,DB_3E,DB_3F,
  IN_B_xC,OUT_xC_B,SBC_HL_BC,LD_xWORDe_BC,NEG,RETN,IM_0,LD_I_A,
  IN_C_xC,OUT_xC_C,ADC_HL_BC,LD_BC_xWORDe,DB_4C,RETI,DB_,LD_R_A,
  IN_D_xC,OUT_xC_D,SBC_HL_DE,LD_xWORDe_DE,DB_54,DB_55,IM_1,LD_A_I,
  IN_E_xC,OUT_xC_E,ADC_HL_DE,LD_DE_xWORDe,DB_5C,DB_5D,IM_2,LD_A_R,
  IN_H_xC,OUT_xC_H,SBC_HL_HL,LD_xWORDe_HL,DB_64,DB_65,DB_66,RRD,
  IN_L_xC,OUT_xC_L,ADC_HL_HL,LD_HL_xWORDe,DB_6C,DB_6D,DB_6E,RLD,
  IN_F_xC,DB_71,SBC_HL_SP,LD_xWORDe_SP,DB_74,DB_75,DB_76,DB_77,
  IN_A_xC,OUT_xC_A,ADC_HL_SP,LD_SP_xWORDe,DB_7C,DB_7D,DB_7E,DB_7F,
  DB_80,DB_81,DB_82,DB_83,DB_84,DB_85,DB_86,DB_87,
  DB_88,DB_89,DB_8A,DB_8B,DB_8C,DB_8D,DB_8E,DB_8F,
  DB_90,DB_91,DB_92,DB_93,DB_94,DB_95,DB_96,DB_97,
  DB_98,DB_99,DB_9A,DB_9B,DB_9C,DB_9D,DB_9E,DB_9F,
  LDI,CPI,INI,OUTI,DB_A4,DB_A5,DB_A6,DB_A7,
  LDD,CPD,IND,OUTD,DB_AC,DB_AD,DB_AE,DB_AF,
  LDIR,CPIR,INIR,OTIR,DB_B4,DB_B5,DB_B6,DB_B7,
  LDDR,CPDR,INDR,OTDR,DB_BC,DB_BD,DB_BE,DB_BF,
  DB_C0,DB_C1,DB_C2,DB_C3,DB_C4,DB_C5,DB_C6,DB_C7,
  DB_C8,DB_C9,DB_CA,DB_CB,DB_CC,DB_CD,DB_CE,DB_CF,
  DB_D0,DB_D1,DB_D2,DB_D3,DB_D4,DB_D5,DB_D6,DB_D7,
  DB_D8,DB_D9,DB_DA,DB_DB,DB_DC,DB_DD,DB_DE,DB_DF,
  DB_E0,DB_E1,DB_E2,DB_E3,DB_E4,DB_E5,DB_E6,DB_E7,
  DB_E8,DB_E9,DB_EA,DB_EB,DB_EC,DB_ED,DB_EE,DB_EF,
  DB_F0,DB_F1,DB_F2,DB_F3,DB_F4,DB_F5,DB_F6,DB_F7,
  DB_F8,DB_F9,DB_FA,DB_FB,DB_FC,DB_FD,DB_FE,DB_FF
};
