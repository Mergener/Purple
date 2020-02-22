#include "purplethread.h"

#include <stdexcept>

#include "opcodes.h"

namespace purple {

PurpleThread::~PurpleThread() {
}

void PurpleThread::init(const Assembly* as) {
	if (m_Running)
		return;

	m_CurrStackIdx = 0;
	m_MainAsm = as;
	m_CurrAsm = as;
	m_Running = true;
	m_CurrReader = as->getInstructionReader();
}

void PurpleThread::tick() {
	if (!m_Running)
		return;

	Opcode op;
	m_CurrReader.readUnsafe(reinterpret_cast<std::uint16_t*>(&op));

	switch (op) {
		
		/*****************************/		

		case Opcode::Add: {
			std::uint16_t op1Offset, op2Offset;

			m_CurrReader.readUnsafe(&op1Offset);
			m_CurrReader.readUnsafe(&op2Offset);

			m_Stack[m_CurrStackIdx + op1Offset].pLong += m_Stack[m_CurrStackIdx + op1Offset].pLong;

			break;
		}
		case Opcode::Sub: {
			std::uint16_t op1Offset, op2Offset;

			m_CurrReader.readUnsafe(&op1Offset);
			m_CurrReader.readUnsafe(&op2Offset);

			m_Stack[m_CurrStackIdx + op1Offset].pLong -= m_Stack[m_CurrStackIdx + op1Offset].pLong;

			break;
		}
		case Opcode::Mul: {
			std::uint16_t op1Offset, op2Offset;

			m_CurrReader.readUnsafe(&op1Offset);
			m_CurrReader.readUnsafe(&op2Offset);

			m_Stack[m_CurrStackIdx + op1Offset].pLong *= m_Stack[m_CurrStackIdx + op1Offset].pLong;

			break;
		}
		case Opcode::Umul: {
			std::uint16_t op1Offset, op2Offset;

			m_CurrReader.readUnsafe(&op1Offset);
			m_CurrReader.readUnsafe(&op2Offset);

			m_Stack[m_CurrStackIdx + op1Offset].pLong *= m_Stack[m_CurrStackIdx + op1Offset].pLong;

			break;
		}
		case Opcode::Div: {
			std::uint16_t op1Offset, op2Offset;

			m_CurrReader.readUnsafe(&op1Offset);
			m_CurrReader.readUnsafe(&op2Offset);

			m_Stack[m_CurrStackIdx + op1Offset].pLong /= m_Stack[m_CurrStackIdx + op1Offset].pLong;

			break;
		}
		case Opcode::Udiv: {
			std::uint16_t op1Offset, op2Offset;

			m_CurrReader.readUnsafe(&op1Offset);
			m_CurrReader.readUnsafe(&op2Offset);

			m_Stack[m_CurrStackIdx + op1Offset].pLong /= m_Stack[m_CurrStackIdx + op1Offset].pLong;

			break;
		}
		
		/*****************************/

		case Opcode::CallNative: {
			std::uint32_t fnIdx;
			m_CurrReader.readUnsafe(&fnIdx);

			m_CallFrame.stackView = &m_Stack.data()[m_CurrStackIdx];
			m_CurrAsm->getNativeFuncs()[fnIdx].fnPtr(m_CallFrame);

			break;
		}
		case Opcode::Pushconst: {
			PurpleNative constant;
			m_CurrReader.readUnsafe(&constant.pUint);
			m_Stack.push_back(constant);
			break;
		}
		case Opcode::Pushframe: {
			m_FrameStack.push(m_CurrStackIdx);
			m_CurrStackIdx = m_Stack.size();
			break;
		}
		
		case Opcode::Popframe: {
			m_CurrStackIdx = m_FrameStack.top();
			m_FrameStack.pop();
			break;
		}
		
		/*****************************/
		
		case Opcode::Exit: {
			m_Running = false;
			break;
		}
		case Opcode::CallExtern: {
			std::uint64_t fnAddr;
			std::uint32_t depIdx;

			m_CurrReader.readUnsafe(&fnAddr);
			m_CurrReader.readUnsafe(&depIdx);

			saveCurrChkPoint();

			const Assembly* const newAsm = m_CurrAsm->getDependencies()[depIdx].asmDependency;

			m_CurrAsm = newAsm;
			m_CurrReader = newAsm->getInstructionReader();
			break;
		}
		case Opcode::CallLocal: {
			std::uint64_t fnAddr;
			
			m_CurrReader.readUnsafe(&fnAddr);

			saveCurrChkPoint();

			m_CurrReader.seekposUnsafe(fnAddr);
			break;
		}
		case Opcode::Return: {
			CheckPoint& c = m_ChkPoints.top();
			m_CurrAsm = c.assembly;
			m_CurrReader = c.reader;
			m_ChkPoints.pop();
			break;
		}
	}
}

bool PurpleThread::isRuning() const {
	return m_Running;
}

void PurpleThread::saveCurrChkPoint() {
	CheckPoint c;
	c.assembly = m_CurrAsm;
	c.reader = m_CurrReader;
	m_ChkPoints.push(c);
}	

} // Purple