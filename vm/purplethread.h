#ifndef PURPLETHREAD_H
#define PURPLETHREAD_H

#include <cstdint>
#include <stack>
#include <vector>

#include "assembly.h"
#include "nativetypes.h"

namespace purple {

class PurpleThread {
public:
	void init(const Assembly* as);
	void tick();

	bool isRuning() const;

	PurpleThread() = default;
	~PurpleThread();
	
private:
	const Assembly* m_MainAsm;
	const Assembly* m_CurrAsm;
	BytecodeReader m_CurrReader;

	struct CheckPoint {
		const Assembly* assembly;
		BytecodeReader reader;
	};

	std::stack<std::size_t> m_FrameStack;
	std::stack<CheckPoint> m_ChkPoints;

	bool m_Running = false;

	std::vector<PurpleNative> m_Stack;
	std::size_t m_CurrStackIdx;

	CallFrame m_CallFrame;

	void saveCurrChkPoint();
};


} // Purple


#endif // PURPLETHREAD_H