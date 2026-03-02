#pragma once
#include <thread>
#include <atomic>
#include <optional>
#include <chrono>
#include "WindowsWifiScanner.h"

class BackgroundScanner {
	public:
		BackgroundScanner(std::unique_ptr<WindowsWifiScanner> scanner) : scanner(std::move(scanner)) { }
		void Start();
		void Stop();
		[[nodiscard]] std::optional<std::vector<WifiAccessPoint>> TakeResults();
		void SetScanInterval(std::chrono::seconds seconds);
	private:
		std::unique_ptr<WindowsWifiScanner> scanner;
		std::vector<WifiAccessPoint> scanResults;
		std::thread scanThread;
		std::atomic<bool> running = false;
		std::mutex resultsMtx;
		std::mutex intervalMtx;
		std::condition_variable intervalCv;
		std::chrono::seconds interval{5};
		bool hasResults = false;
		void ThreadWorker();
};