# orm1-proj-el-lib
ORM1 project: electronic library


Elektronska biblioteka 

Realizovati primer namenskog sistema elektronske biblioteke pomoću TCP protokola. Primer se sastoji iz klijentske strane (korisnik) i serverske strane (elektronska biblioteka). Klijent preko tastature može da zadaje sledeće komande koje se šalju serveru:

 • Login – Prijava/Registracija korisnika (npr. putem korisničkog imena ili broja članske kartice i korisničke šifre). 
 
 • Logout – Odjava korisnika.
 
 • Search – Pretraga dostupnih sadržaja elektronske biblioteke.
 
 • SearchAll - Pretraga svih sadržaja elektronske biblioteke.
 
 • Search [id:ID] [author:AUTHOR] [truncated publication name:NAME] [year:YEAR] – Pretraga za zadate kriterijume (sve ili pojedinačno navedene).
 
 • CheckStatus - Provera dobavljenih sadržaja prijavljenog korisnika.
 
 • Download[id:ID] – Dobavljanje sadržaja/izdanja od strane prijavljenog korisnika po ID-u izdanja (download izdanja sa servera na korisnički računar).
 
 
Server na date komande šalje odgovarajuće odgovore koji se prikazuju na ekranu klijentske aplikacije.

Napomene:

 • Svako izdanje elektronske biblioteke poseduje jedinstven ID.
 
 • Svaki korisnik elektronske biblioteke prilikom procesa registracije dobija jedinstven broj članske kartice.
 
 • N korisnika može istovremeno da dobavlja sadržaj istog izdanja elektronske biblioteke.
 
