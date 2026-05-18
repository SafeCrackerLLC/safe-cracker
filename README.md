# Safe Cracker
## Opis
Aplikacija koja na ekranu prikazuje bravu sefa koju je potrebno okretati desno ili lijevo kako bi se otkuljučao sef. Puhanje bravu okreće desno, dok udisanje okreće bravu lijevo. Aplikacija bi imala više težina i mogućnost slanja razultata WiFi-om. Hardware će biti zatvoren u ručno izrađeno kučište.

## Komponente
| Komponenta     | Dimenzije (cm)          | Cijena |
|----------------|-------------------------|--------|
| ESP32          | 1.6 x 5.5 (10) x 3     | 5€     |
| Potenciometar  | 3 x 1.2 x 1.2           | 1€     |
| Pasivni Zujalica | 1.2 x 1.2 x 1.8       | 1€     |
| Joystick       | 3.5 x 4 (5) x 3         | 3€     |
| OLED Ekran     | 2.4 x 2.8 x 0.5         | -      |
| Napajanje      | 2.4 x 3.2 (8) x 5.2     | 8€     |
| Baterija       | 5 x 1.7 x 2.5           | 4€     |


## Funkcionalnosti
- **Prikaz brave** — OLED ekran prikazuje animaciju brave sefa
- **Rotacija desno** — puhanje u mikrofon okreće bravu u desno  (simulirano potenciometrom)
- **Rotacija lijevo** — udisanje okreće bravu u lijevo          (simulirano potenciometrom)
- **Više težina** — različite kombinacije i duljine za različite razine težine
- **Slanje rezultata** — WiFi webhoook za prijenos rezultata (vrijeme rješavanja, težina, ime)
- **Zvučni feedback** — buzzer signalizira točne/netočne poteze i otključavanje
- **Kućište** — sav hardware zatvoren u ručno izrađenom kučištu