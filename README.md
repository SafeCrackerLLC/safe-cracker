# Safe Cracker

> Interaktivna ESP32 igra otključavanja sefa s OLED prikazom, fizičkim kontrolama i WiFi leaderboardom.

# Opis projekta

Ovaj projekt je rezultat timskog rada u sklopu projektnog zadatka kolegija **Razvoj ugradbenih sustava** na **Tehničkom veleučilištu u Zagrebu**.

**Safe Cracker** je interaktivna igra otključavanja sefa izrađena pomoću ESP32 mikrokontrolera, OLED zaslona i fizičkih kontrola. Igrač mora precizno okretati bravu sefa kako bi pogodio zadane ciljeve i otključao sef.

Cilj projekta bio je izraditi funkcionalan ugradbeni sustav koji povezuje hardver, softver, korisničko sučelje i bežičnu komunikaciju. Projekt uključuje prikaz igre na OLED zaslonu, upravljanje pomoću fizičkih kontrola, zvučni feedback, mjerenje vremena, izračun rezultata te slanje rezultata na web leaderboard.

Motivacija projekta bila je izraditi zabavan uređaj koji prikazuje primjenu ESP32 mikrokontrolera u interaktivnoj igri. Projekt simulira okretanje brave sefa, pri čemu se puhanje i udisanje u prototipu mogu simulirati potenciometrom.

# Funkcijski zahtjevi

> Ključni zahtjevi projekta su:

* prikaz igre na OLED zaslonu
* upravljanje igrom pomoću fizičkih kontrola
* simulacija otključavanja sefa
* simulacija puhanja i udisanja pomoću potenciometra
* izbor razine težine
* mjerenje vremena rješavanja
* izračun i prikaz rezultata
* zvučni feedback pomoću pasivne zujalice
* WiFi konfiguracija uređaja
* slanje rezultata na web servis
* prikaz rezultata na web leaderboardu
* mogućnost rada u online i offline načinu rada
* lokalno generiranje dokumentacije pomoću Doxygena
* automatsko objavljivanje dokumentacije pomoću GitHub Actions i GitHub Pages

# Tehnologije

* ESP32
* Arduino/C++
* OLED zaslon
* I2C komunikacija
* potenciometar
* joystick
* tipka
* pasivna zujalica
* WiFi
* Python
* Flask
* SQLite
* HTML
* CSS
* JavaScript
* Doxygen
* GitHub Actions
* GitHub Pages

# Instalacija

Kloniranje repozitorija:

```bash
git clone https://github.com/SafeCrackerLLC/safe-cracker.git
cd safe-cracker
```

Za rad s ESP32 dijelom projekta potrebno je imati instalirano Arduino razvojno okruženje ili drugo kompatibilno okruženje za razvoj ESP32 aplikacija.

Python webhook i leaderboard nalaze se u direktoriju `webpage/`.

Pokretanje web servisa:

```bash
podman compose -f webpage/compose.yaml up -d --build
```

Doxygen dokumentacija može se generirati lokalno naredbom:

```bash
doxygen Doxyfile
```

HTML dokumentacija generira se u:

```text
docs/html/index.html
```

GitHub Actions workflow `.github/workflows/doxygen.yml` generira dokumentaciju na `main` grani i objavljuje je preko GitHub Pages.

# Članovi tima

> Popis članova tima, linkovi i glavni doprinos.

| Član tima   | Glavni doprinos                     |
| ----------- | ----------------------------------- |
| Luka Šarić | ESP32 logika igre                   |
| David Gulić | Web leaderboard i backend           |
| Leo Znika | Dokumentacija, testiranje i kućište |

# Kontribucije

> Pravila ovise o organizaciji tima i često su izdvojena u `CONTRIBUTING.md`.

Pravila za doprinos projektu definirana su u dokumentu [`CONTRIBUTING.md`](CONTRIBUTING.md).

Članovi tima dogovorili su podjelu rada prema funkcionalnim cjelinama projekta. Komunikacija se odvija putem dogovorenih kanala tima, a promjene se prate kroz GitHub repozitorij.

Prilikom rada na projektu očekuje se:

* poštivanje dogovorene strukture projekta
* pisanje razumljivog i održivog koda
* dokumentiranje važnih dijelova sustava
* korištenje GitHub pull requestova ili dogovorenog načina spajanja promjena
* međusobno poštovanje članova tima
* poštivanje kodeksa ponašanja

# 📝 Kodeks ponašanja [![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](CODE_OF_CONDUCT.md)

Kao studenti upoznati smo s minimumom prihvatljivog ponašanja definiranim u kodeksu ponašanja studenata.

Očekujemo poštivanje [etičkog kodeksa IEEE-a](https://www.ieee.org/about/corporate/governance/p7-8.html), koji ima važnu obrazovnu funkciju postavljanja visokih standarda integriteta, odgovornog ponašanja i etičkog djelovanja u profesionalnim aktivnostima.

Kodeks ponašanja skup je provedivih pravila koja služe za jasnu komunikaciju očekivanja i zahtjeva za rad zajednice ili tima. Njime se definiraju obaveze, prava, neprihvatljiva ponašanja te odgovarajuće posljedice.

U ovom repozitoriju koristi se jedan od široko prihvaćenih kodeksa ponašanja za rad u zajednici otvorenog koda: [`Contributor Covenant`](CODE_OF_CONDUCT.md).

> ### Poboljšanje funkcioniranja tima:
>
> * definirati način na koji će rad biti podijeljen među članovima grupe
> * dogovoriti kako će grupa međusobno komunicirati
> * ne gubiti vrijeme na dogovore o rješavanju sporova, već primijeniti postojeće standarde
> * podrazumijeva se da će svi članovi grupe slijediti kodeks ponašanja

Neprihvatljivo ponašanje može se prijaviti održavateljima projekta putem kontakt adrese navedene u dokumentu [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md).

# 📝 Licenca

Ovaj projekt licenciran je pod **GNU General Public License v3.0** licencom.

GNU GPL v3.0 omogućuje korištenje, proučavanje, mijenjanje i dijeljenje programskog koda, uz uvjet da se izvedene verzije distribuiraju pod istim licencnim uvjetima.

Detalji licence nalaze se u datoteci [`LICENSE`](LICENSE).

> ### Napomena:
>
> Svi paketi, biblioteke i vanjski alati korišteni u projektu distribuiraju se pod vlastitim licencama.
> Svi upotrijebljeni materijali, uključujući slike, modele, animacije i druge resurse, distribuiraju se pod vlastitim licencama.
>
> Originalni predložak README dokumenta korišten u sklopu kolegija licenciran je pod CC0 1.0 Universal licencom.

Originalni predložak:

[![cc0-1.0][cc0-1.0-shield]][cc0-1.0]

> COPYING: All the content within the original template repository is dedicated to the public domain under the CC0 1.0 Universal Public Domain Dedication.

[![CC0-1.0][cc0-1.0-image]][cc0-1.0]

# Reference na licenciranje repozitorija

* [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)
* [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/deed.hr)

[cc0-1.0]: https://creativecommons.org/publicdomain/zero/1.0/deed.hr
[cc0-1.0-image]: https://licensebuttons.net/p/zero/1.0/88x31.png
[cc0-1.0-shield]: https://img.shields.io/badge/License-CC0--1.0-lightgrey.svg
