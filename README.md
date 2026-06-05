# Safe Cracker

@brief ESP32 igra sefa sa OLED prikazom, fizickim kontrolama i WiFi leaderboardom.

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

## WiFi i leaderboard
- ESP32 na bootu proba spremljeni WiFi. Ako nema credentiala ili spajanje ne uspije, otvara hotspot `SafeCracker-XXXX` sa sifrom `safe1234`.
- Nakon spajanja na hotspot otvori `http://192.168.4.1` i upisi SSID/password i webhook URL, ili odaberi offline mode. Default webhook je `https://ruc-esp.nerizz.com/api/scores`.
- Glavni meni ima `Network info` za pregled SSID-a/webhooka i otvaranje novog setupa te `Go offline`/`Go online`.
- Nakon uspjesno rijesenog levela ESP32 salje `POST` na spremljeni webhook s levelom, vremenom, stabilnosti i brojem pogodjenih targeta.
- Python webhook i leaderboard su u `webpage/`; server se pokrece preko `podman compose -f webpage/compose.yaml up -d --build`.

## Doxygen dokumentacija
- Lokalno generiranje dokumentacije: `doxygen Doxyfile`
- HTML izlaz se generira u `docs/html/index.html`
- GitHub Actions workflow `.github/workflows/doxygen.yml` generira dokumentaciju na `main` grani i objavljuje je preko GitHub Pages.
