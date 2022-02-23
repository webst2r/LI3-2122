 #include <stdlib.h>
 #include <time.h>
 int ValidDateFormat(int ano, int mes, int dia) {
    struct tm tm1 = { 0 };
    tm1.tm_year = ano - 1900;
    tm1.tm_mon = mes - 1;
    tm1.tm_mday = dia;
    struct tm tm2 = tm1;
    if (mktime(&tm1) == -1) return 0; // falhou a conversao.
    // O mktime() ajustou os campos?
    if (tm1.tm_year != tm2.tm_year) return 0;
    if (tm1.tm_mon != tm2.tm_mon) return 0;
    return tm1.tm_mday == tm2.tm_mday;
}

int ValidDate (struct tm date){
    /* Testar se a data é no FUTURO*/
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    time_t tempo_agora = mktime(&tm);
    time_t tempo_data = mktime(&date);
    double diffSecs = difftime(tempo_agora, tempo_data); // Se for negativo, entao tempo_agora < tempo_data
    if(diffSecs < 0) return 0;

    //if(date.tm_hour == 0 && date.tm_min == 0 && date.tm_sec == 0) return 0;
    if((date.tm_year < 105) || ((date.tm_year == 105) && (date.tm_mon < 3)) || ((date.tm_year == 105) && (date.tm_mon == 3) && (date.tm_mday < 7))) return 0;

    return 1;
}