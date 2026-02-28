#include "BackgroundScanner.h"

void BackgroundScanner::Start() {
	running = true;
	scanThread = std::thread(&BackgroundScanner::ThreadWorker, this);
}

void BackgroundScanner::Stop() {
	running = false;
	intervalCv.notify_all();
	if (scanThread.joinable())
		scanThread.join();
}

std::optional<std::vector<WifiAccessPoint>> BackgroundScanner::TakeResults() {
	std::lock_guard<std::mutex> lock(resultsMtx);
	if (!hasResults) return std::nullopt;
	hasResults = false;
	return std::move(scanResults);
}

void BackgroundScanner::SetScanInterval(std::chrono::seconds seconds) {
	interval = seconds;
}

void BackgroundScanner::ThreadWorker() {
	while (running) {
		auto start = std::chrono::steady_clock::now();
		auto networks = scanner->ScanNetworks();

		{
			std::lock_guard<std::mutex> lock(resultsMtx);
			scanResults = std::move(networks);
			hasResults = true;
		}

		std::unique_lock<std::mutex> lock(intervalMtx);
		intervalCv.wait_until(lock, start + interval, [this] { return !running; });
	}
}

