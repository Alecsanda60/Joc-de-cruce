#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void Impartire_carti (int *carti_amestecate, int carti_jucatori[4][6])
{
    srand(time(NULL));
    
    for (int i = 22; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = carti_amestecate[i];
        carti_amestecate[i] = carti_amestecate[j];
        carti_amestecate[j] = temp;
    }

    for(int i = 0; i < 24; i=i+4)
    {
        carti_jucatori[1][i/4] = carti_amestecate[i];
        carti_jucatori[2][i/4] = carti_amestecate[i+1];
        carti_jucatori[3][i/4] = carti_amestecate[i+2];
        carti_jucatori[0][i/4] = carti_amestecate[i+3];
    }
}

void decodor (int carte)
{
    if(carte!=-1)
    {
        printf("%d ",carte);

        if(carte%6==0)
        printf("nouar ");
        else if(carte%6==5)
        printf("as ");
        else if(carte%6==1)
        printf("doiar ");
        else if(carte%6==2)
        printf("treiar ");
        else if(carte%6==3)
        printf("patrar ");
        else if(carte%6==4)
        printf("zeca ");

        if(carte/6==0)
            printf("de inima\n");
        else if(carte/6==1)
            printf("de frunza\n");
        else if(carte/6==2)
            printf("de ghinda\n");
        else if(carte/6==3)
            printf("de duba\n");
    }
}

int decodor_punctaj (int carte)
{
    if(carte!=-1)
    {
        if(carte%6==0)
        return 0;
        else if(carte%6==5)
        return 11;
        else if(carte%6==1)
        return 2;
        else if(carte%6==2)
        return 3;
        else if(carte%6==3)
        return 4;
        else if(carte%6==4)
        return 10;
    }
    return 0;
}

int cine_le_ia(int masa[8], int tromf)
{
    int max=0;
    int max_tromf=0;
    int cine_le_ia=0;
    int joc_de=masa[0]/6;
    for(int i=0; i<4; i++)
    {
        if(masa[i]/6==joc_de)
        {
            if(masa[i]%6>max)
            {
                max=masa[i]%6;
                cine_le_ia=i;
            }
        }
        else
        {
            if(masa[i]/6==tromf && joc_de!=tromf)
            {
                if(masa[i]%6>max_tromf)
                {
                    max_tromf=masa[i]%6;
                    cine_le_ia=i;
                }
            }
        }
    }
    return cine_le_ia;
}

void stergerea_cartii_alese(int carti_jucatori[4][6], int masa[8])
{
    for(int k=0; k<4;k++)
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<6; j++)
            {
                if(carti_jucatori[i][j]==masa[k])
                {
                    carti_jucatori[i][j]=-1;
                    
                }
            }
        }
    }
}

int main(void)
{
    int carti_de_joc[24];
    for (int i = 0; i < 24; i++)
    {
        carti_de_joc[i] = i;
    }

    int punctaj_ales;
    printf("Pana la ce scor jucati?\n(Variante uzuale: 11 sau 21)\n");
    scanf("%d", &punctaj_ales);

    int carti_jucatori[4][6];
    int punctaj_echipa_impara=0, punctaj_echipa_para=0;
    int runda=1;
    int ajutor_masa[8];

    ajutor_masa[0]=0;
    ajutor_masa[1]=1;
    ajutor_masa[2]=2;
    ajutor_masa[3]=3;
    ajutor_masa[4]=0;
    ajutor_masa[5]=1;
    ajutor_masa[6]=2;
    ajutor_masa[7]=3;

    while(punctaj_echipa_impara < punctaj_ales && punctaj_echipa_para < punctaj_ales)
    {
        Impartire_carti(carti_de_joc, carti_jucatori);
        
        int cate_faci[4];
        int cine_va_face;

        printf("Cartile jucatorului %d:\n", runda%4);
        for (int i = 0; i < 6; i++)
        {
            decodor(carti_jucatori[runda%4][i]);
        }
        printf("\n");
            
        printf("Jucatorul %d cate faci?\n(Optiuni posibile: 0, 1, 2, 3, 4, 5, 6)\n", runda%4);
        scanf("%d", &cate_faci[runda%4]);
        cine_va_face=runda%4;

        printf("Cartile jucatorului %d:\n", (runda+1)%4);
        for (int i = 0; i < 6; i++)
        {
           decodor(carti_jucatori[(runda+1)%4][i]);
        }
        printf("\n");
            
        printf("Jucatorul %d cate faci?\n(Optiuni posibile: 0, 1, 2, 3, 4, 5, 6)\n", (runda+1)%4);
        scanf("%d", &cate_faci[(runda+1)%4]);
        if(cate_faci[(runda+1)%4]>cate_faci[runda%4])
            cine_va_face=(runda+1)%4;

        printf("Cartile jucatorului %d:\n", (runda+2)%4);
        for (int i = 0; i < 6; i++)
        {
            decodor(carti_jucatori[(runda+2)%4][i]);
        }
        printf("\n");
            
        printf("Jucatorul %d cate faci?\n(Optiuni posibile: 0, 1, 2, 3, 4, 5, 6)\n", (runda+2)%4);
        scanf("%d", &cate_faci[(runda+2)%4]);
        if(cate_faci[(runda+2)%4]>cate_faci[(runda+cine_va_face-1)%4])
            cine_va_face=(runda+2)%4;

        printf("Cartile jucatorului %d:\n", (runda+3)%4);
        for (int i = 0; i < 6; i++)
        {
            decodor(carti_jucatori[(runda+3)%4][i]);
        }
        printf("\n");
            
        printf("Jucatorul %d cate faci?\n(Optiuni posibile: 0, 1, 2, 3, 4, 5, 6)\n", (runda+3)%4);
        scanf("%d", &cate_faci[(runda+3)%4]);
        if(cate_faci[(runda+3)%4]>cate_faci[(runda+cine_va_face-1)%4])
            cine_va_face=(runda+3)%4; 
        
        printf("Cine va face: Jucatorul %d\n", cine_va_face);
        printf("Cate face: %d\n", cate_faci[(runda+cine_va_face-1)%4]);

        int masa[8], punctaj_pe_masa=0;
        int imparii = 0, parii = 0;
        int tromf;

        printf("Jucatorul %d ce carte pui pe masa?\n", (runda+cine_va_face-1)%4);
        for (int i = 0; i < 6; i++)
        {
            decodor(carti_jucatori[(runda+cine_va_face-1)%4][i]);
        }
        scanf("%d", &masa[0]);
        masa[4]=masa[0];
        tromf=masa[0]/6;

        for(int j=1; j<4; j++)
        {
            printf("Jucatorul %d ce carte pui pe masa?\n", ajutor_masa[j+cine_va_face]);
            for (int i = 0; i < 6; i++)
            {
                decodor(carti_jucatori[ajutor_masa[j+cine_va_face]][i]);
            }
            scanf("%d", &masa[j]);
            masa[j+4]=masa[j];
        }

        stergerea_cartii_alese(carti_jucatori, masa);

        printf("Cartile de pe masa sunt:\n");
        for(int j=0; j<4; j++)
        {
            punctaj_pe_masa=punctaj_pe_masa+decodor_punctaj(masa[j]);
            decodor(masa[j]);
        }

        if(((cine_le_ia(masa,tromf)+cine_va_face)%4)%2==1)
        {
            printf("Echipa imparilor a castigat tura\n");
            imparii=imparii+punctaj_pe_masa;
        }
        else
        {
            printf("Echipa parilor a castigat tura\n");
            parii=parii+punctaj_pe_masa;
        }
        punctaj_pe_masa=0;
        printf("punctaj impari: %d, %d\n",imparii,punctaj_echipa_impara);
        printf("punctaj pari: %d, %d\n",parii,punctaj_echipa_para);
        cine_va_face=(cine_le_ia(masa,tromf)+cine_va_face)%4;

        for(int i=0;i<5;i++)
        {
            printf("Jucatorul %d ce carte pui pe masa?\n", (runda+cine_va_face-1)%4);
            for (int i = 0; i < 6; i++)
            {
                decodor(carti_jucatori[(runda+cine_va_face-1)%4][i]);
            }
            scanf("%d", &masa[0]);
            masa[4]=masa[0];
    
            for(int j=1; j<4; j++)
            {
                printf("Jucatorul %d ce carte pui pe masa?\n", ajutor_masa[j+cine_va_face]);
                for (int i = 0; i < 6; i++)
                {
                    decodor(carti_jucatori[ajutor_masa[j+cine_va_face]][i]);
                }
                scanf("%d", &masa[j]);
                masa[j+4]=masa[j];
            }

            stergerea_cartii_alese(carti_jucatori, masa);

            printf("Cartile de pe masa sunt:\n");
            for(int j=0; j<4; j++)
            {
                punctaj_pe_masa=punctaj_pe_masa+decodor_punctaj(masa[j]);
                decodor(masa[j]);
            }
            if(((cine_le_ia(masa,tromf)+cine_va_face)%4)%2==1)
            {
                printf("Echipa imparilor a castigat tura\n");
                imparii=imparii+punctaj_pe_masa;
            }
            else
            {
                printf("Echipa parilor a castigat tura\n");
                parii=parii+punctaj_pe_masa;
            }
            punctaj_pe_masa=0;
            printf("punctaj impari: %d, %d\n",imparii,punctaj_echipa_impara);
            printf("punctaj pari: %d, %d\n",parii,punctaj_echipa_para);
            cine_va_face=(cine_le_ia(masa,tromf)+cine_va_face)%4;
        }
            int cine_mare;
            int marele=0;
            for(int i=0; i<4; i++)
            {
                if(cate_faci[i]>marele)
                {
                    marele=cate_faci[i];
                    cine_mare=i;
                }
            }

            if(cine_mare%2==0)
            {
                if(parii/33<marele)
                    {
                        punctaj_echipa_para=punctaj_echipa_para-marele;
                        punctaj_echipa_impara=punctaj_echipa_impara+imparii/33;
                    }
                else
                    {
                        punctaj_echipa_para=punctaj_echipa_para+parii/33;
                        punctaj_echipa_impara=punctaj_echipa_impara+imparii/33;
                    }    
            }
            else
            {
                if(imparii/33<marele)
                    {
                        punctaj_echipa_impara=punctaj_echipa_impara-marele;
                        punctaj_echipa_para=punctaj_echipa_para+parii/33;
                    }
                else
                    {
                        punctaj_echipa_para=punctaj_echipa_para+parii/33;
                        punctaj_echipa_impara=punctaj_echipa_impara+imparii/33;
                    }    
            }
        
    }
    if(punctaj_echipa_impara>punctaj_echipa_para)
        printf("Echipa imparilor a castigat cu %d puncte!\n", punctaj_echipa_impara);
    else if(punctaj_echipa_impara<punctaj_echipa_para)
        printf("Echipa parilor a castigat cu %d puncte!\n", punctaj_echipa_para);
    else
        printf("Remiza!\n");
    
    return 0;
}
