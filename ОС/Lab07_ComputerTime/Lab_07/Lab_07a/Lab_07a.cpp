#include <Windows.h>
#include <iostream>
#include <iomanip>

int main() {
    SYSTEMTIME localTime;
    SYSTEMTIME utcTime;

    // Получаем локальное время
    GetLocalTime(&localTime);

    // Получаем UTC-время
    GetSystemTime(&utcTime);

    // Вычисляем смещение часового пояса в часах
    int tzOffsetHours = static_cast<int>(localTime.wHour) - static_cast<int>(utcTime.wHour);
    int tzOffsetDays = static_cast<int>(localTime.wDay) - static_cast<int>(utcTime.wDay);

    // Учёт смены дня при переходе через полночь
    if (tzOffsetDays == 1 || tzOffsetDays < -1) tzOffsetHours += 24;
    if (tzOffsetDays == -1 || tzOffsetDays > 1) tzOffsetHours -= 24;

    // Определяем знак смещения
    char sign = (tzOffsetHours >= 0) ? '+' : '-';
    if (tzOffsetHours < 0) tzOffsetHours = -tzOffsetHours;

    // Вывод в формате YYYY-MM-DDThh:mm:ss±hh
    std::cout << std::setfill('0')
        << localTime.wYear << '-'
        << std::setw(2) << localTime.wMonth << '-'
        << std::setw(2) << localTime.wDay << 'T'
        << std::setw(2) << localTime.wHour << ':'
        << std::setw(2) << localTime.wMinute << ':'
        << std::setw(2) << localTime.wSecond
        << sign
        << std::setw(2) << tzOffsetHours
        << std::endl;

    return 0;
}
