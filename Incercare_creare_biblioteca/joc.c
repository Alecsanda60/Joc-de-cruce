#include <stdio.h>
#include "Pachet_carti.h"

int main ()
{
  int carti_de_joc[24];
  Generare_pachet(carti_de_joc);
  int punctaj_ales;
    printf("Pana la ce scor jucati?\n(Variante uzuale: 11 sau 21)\n");
    scanf("%d", &punctaj_ales);

    int carti_jucatori[4][6];
    ///int punctaj_echipa_impara=0, punctaj_echipa_para=0;
    int runda=1;
    ///int ajutor_masa[8];
    Impartire_carti(carti_de_joc, carti_jucatori);
    /*printf("Cartile jucatorului %d:\n", runda%4);
        for (int i = 0; i < 6; i++)
        {
            Decodor(carti_jucatori[runda%4][i]);
	    
        }
        printf("\n");
    */
    for(int i=0; i<4 ;i++)
      {
	for(int j=0; j<6; j++)
	  {
	    Decodor(carti_jucatori[i][j]);
	    printf("%d",Decodor_punctaj(carti_jucatori[i][j]));
	  }
	
      }
}
