#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <unistd.h>

#define PORT 5555
#define SERVER_IP "10.0.2.15"

int alegerea_punctajului(int punctaj_ales, int row)
{
    move(row - 1, 22);
    scanw("%d", &punctaj_ales);
    move(row - 1, 22);
    clrtoeol();
    if (punctaj_ales == 11 || punctaj_ales == 21)
    {
        return punctaj_ales;
    }
    else
    {
        mvprintw(row - 2, 0, "Punctaj invalid! Alege 11 sau 21:");
        refresh();
        sleep(3);
        move(row - 6, 0);
        clrtoeol();
        return alegerea_punctajului(punctaj_ales, row);
    }
}

void decodor(int carte, int unde, int unde2)
{
    if (carte != 66)
    {
        mvprintw(unde, unde2 + 0, "%d", carte);

        if (carte % 6 == 0)
            mvprintw(unde, unde2 + 3, "nouar");
        else if (carte % 6 == 5)
            mvprintw(unde, unde2 + 3, "as");
        else if (carte % 6 == 1)
            mvprintw(unde, unde2 + 3, "doiar");
        else if (carte % 6 == 2)
            mvprintw(unde, unde2 + 3, "treiar");
        else if (carte % 6 == 3)
            mvprintw(unde, unde2 + 3, "patrar");
        else if (carte % 6 == 4)
            mvprintw(unde, unde2 + 3, "zeca");

        if (carte / 6 == 0)
            mvprintw(unde, unde2 + 10, "de inima");
        else if (carte / 6 == 1)
            mvprintw(unde, unde2 + 10, "de frunza");
        else if (carte / 6 == 2)
            mvprintw(unde, unde2 + 10, "de ghinda");
        else if (carte / 6 == 3)
            mvprintw(unde, unde2 + 10, "de duba");
    }
    refresh();
}

int verificare_cate_faci(int row)
{
    int cate_faci;
    move(row - 1, 22);
    scanw("%d", &cate_faci);
    move(row - 1, 22);
    clrtoeol();
    if (cate_faci >= 0 && cate_faci <= 6)
    {
        return cate_faci;
    }
    else
    {
        move(row, 0);
        clrtoeol();
        mvprintw(row - 2, 0, "Numar invalid! Alege un numar intre 0 si 6:");
        refresh();
        sleep(3);
        move(row - 6, 0);
        clrtoeol();
        return verificare_cate_faci(row);
    }
}

int decodor_punctaj(int carte)
{
    if (carte != -1)
    {
        if (carte % 6 == 0)
            return 0;
        else if (carte % 6 == 5)
            return 11;
        else if (carte % 6 == 1)
            return 2;
        else if (carte % 6 == 2)
            return 3;
        else if (carte % 6 == 3)
            return 4;
        else if (carte % 6 == 4)
            return 10;
    }
    return -1;
}

int cine_le_ia(int masa[8], int tromf)
{
    int max = 0;
    int max_tromf = -1;
    int cine_le_ia = 0;
    int joc_de = masa[0] / 6;
    for (int i = 0; i < 4; i++)
    {
        if (masa[i] / 6 == joc_de && max_tromf == -1)
        {
            if (masa[i] % 6 > max)
            {
                max = masa[i] % 6;
                cine_le_ia = i;
            }
        }
        else
        {
            if (masa[i] / 6 == tromf / 6 && joc_de != tromf / 6)
            {
                if (masa[i] % 6 > max_tromf)
                {
                    max_tromf = masa[i] % 6;
                    cine_le_ia = i;
                }
            }
        }
    }
    return cine_le_ia;
}

void stergerea_cartii_alese(int carti_jucatori[4][6], int masa[8])
{
    for (int k = 0; k < 4; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (carti_jucatori[i][j] == masa[k])
                {
                    carti_jucatori[i][j] = 66;
                }
            }
        }
    }
}

int verificare_carte_aleasa_sa_fie_a_jucatorului(int carti_jucatori[4][6], int carte_aleasa, int cine_va_face, int row)
{
    for (int i = 0; i < 6; i++)
    {
        if (carti_jucatori[cine_va_face][i] == carte_aleasa && carte_aleasa != 66)
        {
            return carte_aleasa;
        }
    }
    mvprintw(row - 2, 0, "Nu ai aceasta carte in mana!!!Introdu alta carte:");
    refresh();
    sleep(3);
    move(row - 6, 0);
    clrtoeol();
    move(row - 1, 22);
    scanw("%d", &carte_aleasa);
    move(row - 1, 22);
    clrtoeol();
    carte_aleasa = verificare_carte_aleasa_sa_fie_a_jucatorului(carti_jucatori, carte_aleasa, cine_va_face, row);
    return carte_aleasa;
}

int verificare_trisare(int carti_jucatori[4][6], int masa[8], int carte_aleasa, int cine_va_face, int tromf, int row)
{
    carte_aleasa = verificare_carte_aleasa_sa_fie_a_jucatorului(carti_jucatori, carte_aleasa, cine_va_face, row);
    int corect = 1;
    if (masa[0] / 6 == carte_aleasa / 6)
    {
        return carte_aleasa;
        corect = 1;
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            if (carti_jucatori[cine_va_face][i] / 6 == masa[0] / 6)
            {
                corect = 0;
                break;
            }
        }
        if (carte_aleasa / 6 == tromf / 6 && corect != 0)
        {
            corect = 2;
            return carte_aleasa;
        }
        else if (corect == 1)
        {
            for (int i = 0; i < 6; i++)
            {
                if (carti_jucatori[cine_va_face][i] / 6 == tromf / 6)
                {
                    corect = 3;
                    break;
                }
            }
        }
    }
    if (corect == 0)
    {
        mvprintw(row - 2, 0, "Ai carte in mana de acelasi fel ca prima!!!Introdu alta carte:");
        refresh();
        sleep(3);
        move(row - 6, 0);
        clrtoeol();
        move(row - 1, 22);
        scanw("%d", &carte_aleasa);
        move(row - 1, 22);
        clrtoeol();
        carte_aleasa = verificare_trisare(carti_jucatori, masa, carte_aleasa, cine_va_face, tromf, row);
        return carte_aleasa;
    }
    else if (corect == 3)
    {
        mvprintw(row - 2, 0, "Ai carte de tromf in mana!!!Introdu alta carte:");
        refresh();
        sleep(3);
        move(row - 6, 0);
        clrtoeol();
        move(row - 1, 22);
        scanw("%d", &carte_aleasa);
        move(row - 1, 22);
        clrtoeol();
        carte_aleasa = verificare_trisare(carti_jucatori, masa, carte_aleasa, cine_va_face, tromf, row);
        return carte_aleasa;
    }
    else
        return carte_aleasa;
}

void resetare_masa(int masa[8])
{
    for (int i = 0; i < 8; i++)
    {
        masa[i] = 66;
    }
}

int main(void)
{

    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char message[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    initscr();
    echo();
    cbreak();
    curs_set(0);

    int row, col;
    getmaxyx(stdscr, row, col);

    mvprintw(0, 0, "Connected to server!");
    refresh();
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, 2, 0);
    mvprintw(1, 0, "Server: %s", buffer);
    refresh();

    int cine_esti = buffer[0] - 48;

    int ok = 0;
    mvprintw(2, 0, "Se asteapta jucatorii...");
    refresh();
    while (ok == 0)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, 1024, 0);
        if (strcmp(buffer, "Start") == 0)
        {
            ok = 1;
        }
    }
    move(2, 0);
    clrtoeol();
    mvprintw(2, 0, "Jocul a inceput!");
    refresh();
    mvprintw(row - 1, 0, "Aici apare ce scrii:");
    refresh();

    int punctaj_ales;
    if (cine_esti == 0)
    {
        move(2, 0);
        clrtoeol();
        mvprintw(2, 0, "Pana la ce scor jucati?(Variante: 11 sau 21)");
        refresh();
        punctaj_ales = alegerea_punctajului(punctaj_ales, row);
        move(2, 0);
        clrtoeol();
        mvprintw(2, 0, "Jocul se termina cand una dintre echipe ajunge la %d puncte", punctaj_ales);
        refresh();
        send(sock, &punctaj_ales, sizeof(punctaj_ales), 0);
    }
    else
    {
        recv(sock, &punctaj_ales, sizeof(punctaj_ales), 0);
        move(2, 0);
        clrtoeol();
        mvprintw(2, 0, "Jocul se termina cand una dintre echipe ajunge la %d puncte", punctaj_ales);
        refresh();
    }
    memset(buffer, 0, sizeof(buffer));

    int carti_jucatori[4][6];
    int punctaj_echipa_impara = 0, punctaj_echipa_para = 0;
    int runda = 0;

    while (punctaj_echipa_impara < punctaj_ales && punctaj_echipa_para < punctaj_ales)
    {

        recv(sock, carti_jucatori, sizeof(carti_jucatori), 0);
        int cate_faci[4] = {0, 0, 0, 0};
        int cine_va_face = 0;

        for (int i = 0; i < 4; i++)
        {
            int jucator_curent = (runda + i) % 4;
            if (cine_esti == jucator_curent)
            {
                mvprintw(4, 0, "Cartile jucatorului %d:", jucator_curent);
                refresh();
                for (int j = 0; j < 6; j++)
                {
                    decodor(carti_jucatori[jucator_curent][j], 5 + j, 0);
                }
                mvprintw(11, 0, "Jucatorul %d cate faci?(Optiuni posibile: 0, 1, 2, 3, 4, 5, 6)", jucator_curent);
                refresh();
                cate_faci[jucator_curent] = verificare_cate_faci(row);
                if (cate_faci[jucator_curent] > cate_faci[cine_va_face])
                {
                    cine_va_face = jucator_curent;
                }
                send(sock, cate_faci, sizeof(cate_faci), 0);
                send(sock, &cine_va_face, sizeof(cine_va_face), 0);
            }
            else
            {
                recv(sock, cate_faci, sizeof(cate_faci), 0);
                recv(sock, &cine_va_face, sizeof(cine_va_face), 0);
                move(3, 0);
                clrtoeol();
                mvprintw(3, 0, "Jucatorul %d va face %d", cine_va_face, cate_faci[cine_va_face]);
                refresh();
            }
        }

        for (int i = 3; i <= 12; i++)
        {
            move(i, 0);
            clrtoeol();
        }
        mvprintw(3, 0, "Cine va face: Jucatorul %d", cine_va_face);
        refresh();
        mvprintw(4, 0, "Cate face: %d", cate_faci[cine_va_face]);
        refresh();

        int masa[8], punctaj_pe_masa = 0;
        int imparii = 0, parii = 0;
        int tromf = 0, carte_aleasa = 0;
        resetare_masa(masa);

        mvprintw(5, 0, "Jucatorul %d ce carte pui pe masa?", cine_va_face);
        refresh();
        if (cine_esti == cine_va_face)
        {
            for (int i = 0; i < 6; i++)
            {
                decodor(carti_jucatori[cine_va_face][i], 6 + i, 0);
            }
            move(row - 1, 22);
            scanw("%d", &carte_aleasa);
            move(row - 1, 22);
            clrtoeol();
            carte_aleasa = verificare_carte_aleasa_sa_fie_a_jucatorului(carti_jucatori, carte_aleasa, (cine_va_face) % 4, row);
            masa[0] = carte_aleasa;
            masa[4] = masa[0];
            tromf = carte_aleasa;
            send(sock, &tromf, sizeof(tromf), 0);
            send(sock, masa, sizeof(masa), 0);
        }
        else
        {
            recv(sock, &tromf, sizeof(tromf), 0);
            recv(sock, masa, sizeof(masa), 0);
            move(5, col - 30);
            clrtoeol();
            mvprintw(5, col - 30, "Cartile de pe masa sunt:");
            refresh();
            for (int k = 0; k < 4; k++)
            {
                decodor(masa[k], 6 + k, col - 30);
            }
        }
        for (int j = 1; j < 4; j++)
        {
            mvprintw(5, 0, "Jucatorul %d ce carte pui pe masa?", (j + cine_va_face) % 4);
            refresh();
            if (cine_esti == (j + cine_va_face) % 4)
            {
                for (int i = 0; i < 6; i++)
                {
                    decodor(carti_jucatori[(j + cine_va_face) % 4][i], 6 + i, 0);
                }
                move(row - 1, 22);
                scanw("%d", &carte_aleasa);
                move(row - 1, 22);
                clrtoeol();
                carte_aleasa = verificare_trisare(carti_jucatori, masa, carte_aleasa, (j + cine_va_face) % 4, tromf, row);
                masa[j] = carte_aleasa;
                masa[j + 4] = masa[j];
                send(sock, masa, sizeof(masa), 0);
            }
            else
            {
                recv(sock, masa, sizeof(masa), 0);
                move(5, col - 30);
                clrtoeol();
                mvprintw(5, col - 30, "Cartile de pe masa sunt:");
                refresh();
                for (int k = 0; k < 4; k++)
                {
                    decodor(masa[k], 6 + k, col - 30);
                }
            }
        }
        stergerea_cartii_alese(carti_jucatori, masa);

        for (int i = 5; i <= 12; i++)
        {
            move(i, 0);
            clrtoeol();
        }
        mvprintw(5, 0, "Cartile de pe masa sunt:");
        refresh();
        for (int j = 0; j < 4; j++)
        {
            punctaj_pe_masa = punctaj_pe_masa + decodor_punctaj(masa[j]);
            decodor(masa[j], 6 + j, 0);
        }

        if (((cine_le_ia(masa, tromf) + cine_va_face) % 4) % 2 == 1)
        {
            mvprintw(10, 0, "Echipa imparilor a castigat tura");
            refresh();
            imparii = imparii + punctaj_pe_masa;
        }
        else
        {
            mvprintw(10, 0, "Echipa parilor a castigat tura");
            refresh();
            parii = parii + punctaj_pe_masa;
        }
        punctaj_pe_masa = 0;
        mvprintw(0, col - 25, "punctaj impari: %d, %d", imparii, punctaj_echipa_impara);
        mvprintw(1, col - 25, "punctaj pari: %d, %d", parii, punctaj_echipa_para);
        refresh();
        cine_va_face = (cine_le_ia(masa, tromf) + cine_va_face) % 4;
        if (cine_esti == 0)
        {
            send(sock, &cine_va_face, sizeof(cine_va_face), 0);
        }
        resetare_masa(masa);
        mvprintw(row - 2, 0, "Mesajul va disparea dupa 5 secunde");
        refresh();
        sleep(5);
        for (int i = 5; i <= row - 2; i++)
        {
            move(i, 0);
            clrtoeol();
        }

        for (int i = 0; i < 5; i++)
        {
            mvprintw(5, 0, "Jucatorul %d ce carte pui pe masa?", cine_va_face);
            refresh();
            if (cine_esti == cine_va_face)
            {
                for (int j = 0; j < 6; j++)
                {
                    decodor(carti_jucatori[cine_va_face][j], 6 + j, 0);
                }
                move(row - 1, 22);
                scanw("%d", &carte_aleasa);
                move(row - 1, 22);
                clrtoeol();
                carte_aleasa = verificare_carte_aleasa_sa_fie_a_jucatorului(carti_jucatori, carte_aleasa, cine_va_face, row);
                masa[0] = carte_aleasa;
                masa[4] = masa[0];
                send(sock, masa, sizeof(masa), 0);
            }
            else
            {
                recv(sock, masa, sizeof(masa), 0);
                move(5, col - 30);
                clrtoeol();
                mvprintw(5, col - 30, "Cartile de pe masa sunt:");
                refresh();
                for (int k = 0; k < 4; k++)
                {
                    decodor(masa[k], 6 + k, col - 30);
                }
            }
            for (int j = 1; j < 4; j++)
            {
                mvprintw(5, 0, "Jucatorul %d ce carte pui pe masa?", (j + cine_va_face) % 4);
                refresh();
                if ((j + cine_va_face) % 4 == cine_esti)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        decodor(carti_jucatori[(j + cine_va_face) % 4][i], 6 + i, 0);
                    }
                    move(row - 1, 22);
                    scanw("%d", &carte_aleasa);
                    move(row - 1, 22);
                    clrtoeol();
                    carte_aleasa = verificare_trisare(carti_jucatori, masa, carte_aleasa, (j + cine_va_face) % 4, tromf, row);
                    masa[j] = carte_aleasa;
                    masa[j + 4] = masa[j];
                    send(sock, masa, sizeof(masa), 0);
                }
                else
                {
                    recv(sock, masa, sizeof(masa), 0);
                    move(5, col - 30);
                    clrtoeol();
                    mvprintw(5, col - 30, "Cartile de pe masa sunt:");
                    refresh();
                    for (int k = 0; k < 4; k++)
                    {
                        decodor(masa[k], 6 + k, col - 30);
                    }
                }
            }

            stergerea_cartii_alese(carti_jucatori, masa);

            for (int i = 5; i <= 12; i++)
            {
                move(i, 0);
                clrtoeol();
            }
            mvprintw(5, 0, "Cartile de pe masa sunt:");
            refresh();
            for (int j = 0; j < 4; j++)
            {
                punctaj_pe_masa = punctaj_pe_masa + decodor_punctaj(masa[j]);
                decodor(masa[j], 6 + j, 0);
            }
            if (((cine_le_ia(masa, tromf) + cine_va_face) % 4) % 2 == 1)
            {
                mvprintw(10, 0, "Echipa imparilor a castigat tura");
                refresh();
                imparii = imparii + punctaj_pe_masa;
            }
            else
            {
                mvprintw(11, 0, "Echipa parilor a castigat tura");
                refresh();
                parii = parii + punctaj_pe_masa;
            }
            punctaj_pe_masa = 0;
            move(0, col - 25);
            clrtoeol();
            move(1, col - 25);
            clrtoeol();
            mvprintw(0, col - 25, "punctaj impari: %d, %d", imparii, punctaj_echipa_impara);
            mvprintw(1, col - 25, "punctaj pari: %d, %d", parii, punctaj_echipa_para);
            refresh();
            cine_va_face = (cine_le_ia(masa, tromf) + cine_va_face) % 4;
            if (cine_esti == 0)
            {
                send(sock, &cine_va_face, sizeof(cine_va_face), 0);
            }
            resetare_masa(masa);
            mvprintw(row - 2, 0, "Mesajul va disparea dupa 5 secunde");
            refresh();
            sleep(5);
            for (int i = 5; i <= row - 2; i++)
            {
                move(i, 0);
                clrtoeol();
            }
        }

        int cine_mare;
        int marele = 0;
        for (int i = 0; i < 4; i++)
        {
            if (cate_faci[i] > marele)
            {
                marele = cate_faci[i];
                cine_mare = i;
            }
        }

        if (cine_mare % 2 == 0)
        {
            if (parii / 33 < marele)
            {
                punctaj_echipa_para = punctaj_echipa_para - marele;
                punctaj_echipa_impara = punctaj_echipa_impara + imparii / 33;
            }
            else
            {
                punctaj_echipa_para = punctaj_echipa_para + parii / 33;
                punctaj_echipa_impara = punctaj_echipa_impara + imparii / 33;
            }
        }
        else
        {
            if (imparii / 33 < marele)
            {
                punctaj_echipa_impara = punctaj_echipa_impara - marele;
                punctaj_echipa_para = punctaj_echipa_para + parii / 33;
            }
            else
            {
                punctaj_echipa_para = punctaj_echipa_para + parii / 33;
                punctaj_echipa_impara = punctaj_echipa_impara + imparii / 33;
            }
        }

        imparii = 0;
        parii = 0;
        move(0, col - 25);
        clrtoeol();
        move(1, col - 25);
        clrtoeol();
        mvprintw(0, col - 25, "punctaj impari: %d, %d", imparii, punctaj_echipa_impara);
        mvprintw(1, col - 25, "punctaj pari: %d, %d", parii, punctaj_echipa_para);
        refresh();

        mvprintw(row - 2, 0, "Mesajul va disparea dupa 5 secunde");
        refresh();
        sleep(5);
        for (int i = 3; i <= row - 2; i++)
        {
            move(i, 0);
            clrtoeol();
        }

        if (cine_esti == 0)
        {
            send(sock, &punctaj_echipa_impara, sizeof(punctaj_echipa_impara), 0);
            send(sock, &punctaj_echipa_para, sizeof(punctaj_echipa_para), 0);
        }

        runda = runda + 1;
    }
    if (punctaj_echipa_impara > punctaj_echipa_para)
        mvprintw(row / 2, (col / 2) - 20, "Echipa imparilor a castigat cu %d puncte!", punctaj_echipa_impara);
    else if (punctaj_echipa_impara < punctaj_echipa_para)
        mvprintw(row / 2, (col / 2) - 19, "Echipa parilor a castigat cu %d puncte!", punctaj_echipa_para);
    else
        mvprintw(row / 2, (col / 2) - 4, "Remiza!!");

    endwin();
    return 0;
}
