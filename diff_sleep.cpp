#include <chrono>
#include <thread>

int main() {
	uint64_t m_PreviousVsync = 0;
    int32_t targetFrameRate = 60; // 目标 30 fps

	while (1) {
		uint64_t current = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t interval = 1000 * 1000 / targetFrameRate;

		if (m_PreviousVsync + interval > current) {
			uint64_t sleepTimeMs = (m_PreviousVsync + interval - current) / 1000;

			if (sleepTimeMs > 0) {
				// printf("Sleep %llu ms for next VSync.\n", sleepTimeMs);
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
			}

			m_PreviousVsync += interval;
		}
		else {
			m_PreviousVsync = current;
		}
        printf("add some function here\n");
	}

    return 0;
}
