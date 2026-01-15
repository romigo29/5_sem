#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>

int main() {
    struct timeval tv;
    gettimeofday(&tv, nullptr); // получаем текущее время с микросекундами

    time_t utcSeconds = tv.tv_sec;

    // Получаем UTC-время
    struct tm utc_tm;
    gmtime_r(&utcSeconds, &utc_tm);

    // Получаем локальное время
    struct tm local_tm;
    localtime_r(&utcSeconds, &local_tm);

    // Вычисляем смещение часового пояса вручную
    int tzOffsetHours = local_tm.tm_hour - utc_tm.tm_hour;
    int tzOffsetDays = local_tm.tm_mday - utc_tm.tm_mday;

    // Корректировка при переходе через полночь
    if (tzOffsetDays == 1 || tzOffsetDays < -1) tzOffsetHours += 24;
    if (tzOffsetDays == -1 || tzOffsetDays > 1) tzOffsetHours -= 24;

    // Определяем знак
    char sign = (tzOffsetHours >= 0) ? '+' : '-';
    if (tzOffsetHours < 0) tzOffsetHours = -tzOffsetHours;

    // Выводим в формате YYYY-MM-DDThh:mm:ss±hh
    std::cout << std::setfill('0')
              << local_tm.tm_year + 1900 << '-'
              << std::setw(2) << local_tm.tm_mon + 1 << '-'
              << std::setw(2) << local_tm.tm_mday << 'T'
              << std::setw(2) << local_tm.tm_hour << ':'
              << std::setw(2) << local_tm.tm_min << ':'
              << std::setw(2) << local_tm.tm_sec
              << sign << std::setw(2) << tzOffsetHours
              << std::endl;

    return 0;
}
