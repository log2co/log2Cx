/*
* File:   cxvm.h
* Author: aaron.hebert@gmail.com
*
* Created on October 7, 2014, 12:19 AM
*/

#ifndef CXVM_H
#define	CXVM_H

/// Only defined for debugging
#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#endif

#include <cmath>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <map>
#include <unordered_map>
#include <chrono>
#include "types.h"
#include "symtab.h"

namespace cx {
	namespace heap {
		// Memory allocation representation class
		typedef std::shared_ptr<uintptr_t> managedmem;

		class mem_mapping {
		private:
		public:
			managedmem shared_ref;
			size_t size;			// sizeof
			type_code typecode;
			type_form typeform;

			size_t length(void);	// sizeof * count
		};

		typedef void* address;
		typedef std::map<uintptr_t, mem_mapping> malloc_map;
	}

	// Op codes
	enum opcode {
		AALOAD, AASTORE, ACONST_NULL, ALOAD, ANEWARRAY,
		ARETURN, ARRAYLENGTH, ASTORE, ATHROW,
		BALOAD, BASTORE, BEQ, BGE, BGT, BIPUSH, BLE,
		BLT, BNE, CALL, CALOAD, CASTORE, CHECKCAST, D2F, D2I, D2L, DADD, DALOAD,
		DASTORE, DCMP, DCONST, DDIV, DLOAD, DMUL, DNEG, DREM, DRETURN,
		DSTORE, DSUB, DUP, DUP2, DUP2_X1, DUP2_X2, DUP_X1, DUP_X2, F2D, F2I,
		F2L, FADD, FALOAD, FASTORE, FCMP, FCONST, FDIV, FLOAD,
		FMUL, FNEG, FREM, FRETURN, FSTORE, FSUB, GETFIELD, GETSTATIC,
		GOTO, GOTO_W, HALT, I2B, I2C, I2D, I2F, I2L, I2S, IADD, IALOAD,
		IAND, IASTORE, ICMP, ICONST, IDIV, IFEQ, IFNE, IFLT,
		IFGE, IFGT, IFLE, IF_ACMPEQ, IF_ACMPNE, IF_ICMPEQ, IF_ICMPNE, IF_ICMPLT,
		IF_ICMPGE, IF_ICMPGT, IF_ICMPLE, IFNONNULL, IFNULL, IINC,
		ILOAD, IMUL, INEG, INOT, INSTANCEOF, INVOKEDYNAMIC, INVOKEFUNCT, INVOKEINTERFACE,
		INVOKESPECIAL, INVOKESTATIC, INVOKEVIRTUAL, IOR, IREM, IRETURN, ISHL, ISHR,
		ISTORE, ISUB, IUSHR, IXOR, JSR, JSR_W, L2D, L2F, L2I, LADD, LALOAD, LAND,
		LASTORE, LCMP, LCONST, LDC, LDC2_W, LDC_W, LDIV,
		LLOAD, LMUL, LNEG, LOOKUPSWITCH, LOR, LREM, LRETURN, LSHL,
		LSHR, LSTORE, LSUB, LUSHR, LXOR, MONITORENTER, MONITOREXIT,
		MULTIANEWARRAY, NEW, NEWARRAY, NOP, PLOAD, POSTOP, POP, POP2, PREOP, PUTFIELD, PUTSTATIC, RET,
		RETURN, SALOAD, SASTORE, SIPUSH, SWAP,
		TABLESWITCH, WALOAD, WASTORE, WIDE
	};

	// Instruction
	typedef struct inst {
		opcode op;
		value arg0;
		value arg1;

		inst() = delete;
		inst(opcode op_) : op(op_){}
		inst(opcode op_, value arg0_) : op(op_), arg0(arg0_) {}
		inst(opcode op_, value arg0_, value arg1_) : op(op_), arg0(arg0_), arg1(arg1_) {}
	} inst;

	// Program instructions
	typedef std::vector<inst> program;
	// Instruction pointer
	typedef program::iterator instr_ptr;

	// Virtual CPU
	typedef struct _vcpu {
		value *stack_ptr;	// Pointer to the current position in stack
		instr_ptr inst_ptr; // Instruction pointer
		program *code_ptr;
		int16_t flag;
	} vcpu;

	enum {
		_STACK_SIZE = 255
	};

	class cxvm {
	private:
		vcpu vpu;					// VPU: Virtual Proc Unit
		heap::malloc_map heap_;		// HEAP: For storing raw memory allocations
		value stack[_STACK_SIZE];	// STACK:

		//std::mutex vm_lock;				// VM lock during execution
		symbol_table_node *p_my_function_id;
		void nano_sleep(int nano_secs);	// Thread sleep while waiting for VM lock

	public:
		
		value *push(void);
		void enter_function(symbol_table_node *p_function_id);
		void go(void);
		cxvm();
		~cxvm(void);

		void lock(void);
		void unlock(void);
	};
}

#endif	/* CXVM_H */
