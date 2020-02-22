#ifndef RUNTIME_H
#define RUNTIME_H

#include <vector>
#include <memory>

#include "purplethread.h"

namespace purple {

class Runtime {	
public:
	void run(int argc, char* argv[]);

	Runtime(Assembly* assembly);

private:
	Assembly* m_MainAsm;

	PurpleThread m_MainThread;

	std::vector<std::unique_ptr<PurpleThread>> m_SecondaryThreads;
};

} // Purple

#endif // RUNTIME_H