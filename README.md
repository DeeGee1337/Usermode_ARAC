# Usermode_ARAC

DAPI – Thema

Projektname: 
Windowsbasierte anti-tampering Software mit Toolerkennung von Reverseengineering-Programmen und  simples Anti-Cheat für Counter Strike: Global Offensive.

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
-	Auslesen und analysieren der Driveroutputs
-	HWID Ban System