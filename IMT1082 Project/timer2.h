#if !defined(__TIMER_H) //  Sikrer at filen kun inkluderes en gang:
#define __TIMER_H

#include <ctime>        //  Struct'en "tm".


class Timer  {
  private:
    int     dager_pr_maaned[12];  //  Antall dager i hver av årets 12 måneder.
    tm*     tidspunkt;            //  Peker til 'tm'-struct.
    time_t  tid;                  //  Avlest tid lagres i denne.
    bool    skuddaar(int aa);     //  3x private/skjulte hjelpefunksjoner:
    int     dagnummer(int da, int ma, int aa);
    void    konverter(int t, int& da, int& ma, int& aa);

  public:
    Timer();
    void hent_tid(int& ti, int& mi, int& se);
    int  hent_dato();
    int  dagnummer(int t);
    int  ukedagsnr(int t);
    int  forskjell_datoer(int t1, int t2);
    int  nestedato(int t);
};

#endif
