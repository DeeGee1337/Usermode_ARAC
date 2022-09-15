# Usermode_ARAC

DAPI – Thema

Projektname: 
Windowsbasierte anti-tampering Software mit Toolerkennung von Reverseengineering-Programmen und  simples Anti-Cheat für Counter Strike: Global Offensive.
Mitglieder: 
Fabian Folger, Anton Halder, Christoph Sommer
Ziel:
Verhindern von Reverseengineering des Spiels um eine unerlaubte Modifikation zu vermeiden.
Erkennung eines frei zugänglichen Cheats anhand von Modulen und Memorydumps.
Generierung einer einzigartigen HardwareID für möglichen, zukünftigen Spielausschluss.
Anti-Cheat executable protection via encryption um die Analyse zu erschweren.

Features:
- Automatisierte Screenshots mit BitBlt (Topmost check)
- Auswertung der Screenshots mit OCR
-	Automatisierte Erkennung der ProzessIDs
-	Parsing der Windowtitles nach gesetzten Flags
-	Terminierung von Reverse-Programme 
-	Erstellung und Auswertung eines Memorydumps
-	Erstellen und Finden von Signaturen in der .rdata Sektion
-	Geladene Module analysieren und bei Auffälligkeiten flaggen
-	Executable Schutz vor Analysen und Reversen 

Zeitkosten: (Insgesamt 450h)
-	Prozesserkennung (20h)
-	Reversing (60h)
-	Research (50h)
-	Windowtitles (10h)
-	Prozessterminierung (10h)
-	Executable Schutz (10h)
-	Memdumps (60h)
-	Signaturen in r.data Sektion (75h)
-	Module durchleuchten (70h)
-	Bugfixing (45h)
-	Testing (20h)
-	Dokumentation (20h)

