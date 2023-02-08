#include "VSyncThread.h"

#include <chrono>
#include <thread>
#include <openvr_driver.h>

#include "Logger.h"

VSyncThread::VSyncThread(int refreshRate)
	: m_bExit(false)
	, m_refreshRate(refreshRate) {}

// Trigger VSync if elapsed time from previous VSync is larger than 30ms.
void VSyncThread::Run() {
	m_PreviousVsync = 0;

	while (!m_bExit) {
		uint64_t current = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t interval = 1000 * 1000 / m_refreshRate;

		if (m_PreviousVsync + interval > current) {
			uint64_t sleepTimeMs = (m_PreviousVsync + interval - current) / 1000;

			if (sleepTimeMs > 0) {
				LOG("Sleep %llu ms for next VSync.\n", sleepTimeMs);
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
			}

			m_PreviousVsync += interval;
		}
		else {
			m_PreviousVsync = current;
		}
        LOG("Generate VSync Event by VSyncThread\n");
		vr::VRServerDriverHost()->VsyncEvent(0);
	}
}

void VSyncThread::Shutdown() {
	m_bExit = true;
}

void VSyncThread::SetRefreshRate(int refreshRate) {
	m_refreshRate = refreshRate;
}
