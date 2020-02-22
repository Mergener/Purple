#include "runtime.h"

namespace purple {

Runtime::Runtime(Assembly* assembly) 
	: m_MainAsm(assembly != nullptr ? assembly : throw std::runtime_error("Null assembly passed to purple runtime.")) {
}

void Runtime::run(int argc, char** argv) {
	m_MainThread.init(m_MainAsm);
	
	while (m_MainThread.isRuning()) {
		m_MainThread.tick();
	}
}

} // Purple