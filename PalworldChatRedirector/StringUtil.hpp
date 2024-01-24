#pragma once

#include <string>
#include <vector>
#include <codecvt>
#include <Windows.h>

class StringUtil {
public:
	static std::wstring utf8_to_wide_winapi(std::string const& src) {
		// https://nekko1119.hatenablog.com/entry/2017/01/02/054629

		auto const dest_size = ::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, nullptr, 0U);
		std::vector<wchar_t> dest(dest_size, L'\0');
		if (::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, dest.data(), (int)dest.size()) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<wchar_t>::length(dest.data()));
		dest.shrink_to_fit();
		return std::wstring(dest.begin(), dest.end());
	}

	static std::string wide_to_utf8_winapi(std::wstring const& src) {
		// https://nekko1119.hatenablog.com/entry/2017/01/02/054629

		auto const dest_size = ::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
		std::vector<char> dest(dest_size, '\0');
		if (::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, dest.data(), (int)dest.size(), nullptr, nullptr) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<char>::length(dest.data()));
		dest.shrink_to_fit();
		return std::string(dest.begin(), dest.end());
	}

	static std::wstring multi_to_wide_winapi(std::string const& src) {
		// https://nekko1119.hatenablog.com/entry/2017/01/02/054629

		auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, nullptr, 0U);
		std::vector<wchar_t> dest(dest_size, L'\0');
		if (::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, dest.data(), (int)dest.size()) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<wchar_t>::length(dest.data()));
		dest.shrink_to_fit();
		return std::wstring(dest.begin(), dest.end());
	}

	static std::string wide_to_multi_winapi(std::wstring const& src) {
		// https://nekko1119.hatenablog.com/entry/2017/01/02/054629

		auto const dest_size = ::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
		std::vector<char> dest(dest_size, '\0');
		if (::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, dest.data(), (int)dest.size(), nullptr, nullptr) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<char>::length(dest.data()));
		dest.shrink_to_fit();
		return std::string(dest.begin(), dest.end());
	}

	static std::string multi_to_utf8_winapi(std::string const& src) {
		// https://nekko1119.hatenablog.com/entry/2017/01/02/054629

		auto const wide = multi_to_wide_winapi(src);
		return wide_to_utf8_winapi(wide);
	}

	static std::string utf8_to_multi_winapi(std::string const& src) {
		// https://nekko1119.hatenablog.com/entry/2017/01/02/054629

		auto const wide = utf8_to_wide_winapi(src);
		return wide_to_multi_winapi(wide);
	}
};
