1. Einleitung:

Die heutigen Terahertz-Technologien versprechen ein rasches Wachstum in den neuen Märkten wie Medizin- und Sicherheitstechnik, zerstörungsfreie Prüfung, Qualitätskontrolle und Gesundheitsvorsorge. Jedoch ist die Geschwindigkeit, mit der neue Märkte entstehen noch langsam, weil verfügbare Technologien hohen Kosten und eine niedrige Ebene der Integration aufweisen, die ihren Masseneinsatz wesentlich einschränken.

Im Lehrstuhl für Hochfrequenzsysteme in der Kommunikationstechnik beschäftigten wir uns intensiv mit der eingebetteten Bildverarbeitung von Terahertz (THz) Video-Kameras. Konkret untersuchten wir die bereits entwickelte und hergestellte THz-Kamera T-LITE M5STICK Plus der Firma M5Stack Technology Co., Ltd., die mit einem THz-Sensor des Typs MLX90640 ausgestattet ist.

Ziel unserer Arbeit war es, die Bilddaten der Kamera zu interpolieren, die von einer Auflösung von 32x24 Pixeln auf eine höhere Auflösung von 135x240 Pixeln skaliert werden sollten. Dies ermöglichte den Vergleich verschiedener Interpolationsmethoden auf zwei unterschiedlichen Plattformen, dem M5Core2 (240x320 Pixel) und dem M5Stick Plus.

Darüber hinaus bestand eine zentrale Aufgabe darin, den Interpolationsalgorithmus aus dem Hauptquellcode des Produkts zu isolieren und separat auf beiden Geräten zu implementieren. Abschließend untersuchten wir die Bildwiederholrate (FPS) für verschiedene Algorithmen auf beiden Plattformen, um die Effizienz und Leistungsfähigkeit der Implementierungen zu vergleichen.

2.	 Hardware:

Im rahmen diesem Praktikum haben wir uns mit Zwei Hardware Beschäftigt: 

- M5STACK CORE2:

![image](https://github.com/user-attachments/assets/5a559da6-89d2-46b9-ae68-ed1d4925ec2b)

- T-LITE M5StickC-PLUS:

![image](https://github.com/user-attachments/assets/fb91fab4-0dec-4566-96d0-fc108368f909)

- M5StickC Plus: 

![image](https://github.com/user-attachments/assets/c28abca7-2350-423b-8cf9-62ed635e43c5)

- Thermal Hat MLX90640:

![image](https://github.com/user-attachments/assets/738464eb-2f33-4ad0-ab95-68ae6023549e)


3. InterpolationAlgorithmen:
   
Anstatt direkt mit den Daten des THz-Sensors zu arbeiten, haben wir zunächst mit einem einfachen Bild begonnen. Dieses Bild hatte eine Auflösung von 32x24 Pixeln, und wir haben es in ein Array umgewandelt, um es dann zu interpolieren.
Unser Ziel war es, das Bild von 32x24 Pixeln auf 135x240 Pixel zu vergrößern. Dafür haben wir verschiedene Interpolationsmethoden verwendet, darunter:

- Bilineare Interpolation.
- Bicubische Interpolation.
- Nearest-Neighbour-Interpolation.
- Isolierte Interpolationsmethode aus dem Original-Quellcode der T-LITE M5StickC Plus.


4.	Implementierung von Interpolationsalgorithmen:

Wir haben verschiedene Interpolationsverfahren implementiert und diese anschließend in die Geräte M5Core2 und M5StickC Plus integriert. Dabei sind wir jedoch auf einige Herausforderungen gestoßen.

-	Umwandlung und Inversion von Bitmap-Daten:

Wir haben das Bild im „Bitmap“-Format, wobei die erste Zeile tatsächlich die letzte im Daten-Array von Bytes ist. Daher müssen wir die Zeilen invertieren. Zusätzlich speichert die Bitmap jeder Pixel in 3 Bytes (B, G und R) im RGB888-Format. Um diese Daten in ein 24-Bit-Farbformat (RGB565) umzuwandeln, verwenden wir eine Funktion namens void(rgb888_to_rgb565), die beide Aufgaben ausführt.

-	Das Speicherproblem:

Zunächst haben wir beim Konvertieren von RGB888 nach RGB565 Speicher für das gesamte Bild zugewiesen, was jedoch nicht möglich war, da der verfügbare Speicher nicht ausreichte. Daher bestand die Lösung darin, nur eine einzelne Zeile zuzuweisen, diese anzuzeigen und dann zur nächsten Zeile überzugehen, um sie zu konvertieren und ebenfalls anzuzeigen, und so weiter.

Abschließend haben wir die Leistung der Algorithmen auf diesen Geräten getestet, indem wir die Bildwiederholrate (FPS) und die Effizienz der Implementierungen miteinander verglichen haben.

5.	Data Visualisieren und Ergebnisse:

   Um einen fairen Vergleich zwischen den beiden Hardwareplattformen, M5Core2 und M5StickC Plus, zu ermöglichen, haben wir unsere Interpolationsalgorithmen auf beiden Geräten getestet. Dabei verwendeten wir eine Bildgröße von 135x240 Pixeln für alle Verfahren. Die folgende Tabelle zeigt die Unterschiede in der Bildwiederholrate (FPS) für die verschiedenen Algorithmen auf den beiden Geräten

![image](https://github.com/user-attachments/assets/6cc1ef01-7788-4ece-8951-33d37197a04b)

![image](https://github.com/user-attachments/assets/e104c755-1488-417f-953d-19b54b8d8176)


Diese Ergebnisse zeigen deutliche Unterschiede in der Performance zwischen den beiden Geräten. Während M5StickC Plus in den meisten Fällen eine höhere Bildwiederholrate erreicht, insbesondere bei der bilinearen und Nearest-Neighbour-Interpolation, zeigt die bicubische Interpolation auf beiden Geräten ähnliche und niedrigere Werte. Die aus dem Original-Quellcode isolierte Interpolation liefert ebenfalls eine signifikant bessere Leistung auf dem M5StickC Plus im Vergleich zum M5Core2.


6.	Analyse der Leistungsunterschiede zwischen M5Core2 und M5StickC Plus:

   
Obwohl wir die gleiche Software auf beiden Geräten, dem M5Core2 und dem M5StickC Plus, verwendet haben, gibt es einen wesentlichen Unterschied in der Leistung. Dieser Unterschied ist nicht auf die Software zurückzuführen, sondern lässt uns über andere mögliche Ursachen nachdenken, die hardwareabhängig sind.
Die Unterschiede in der Leistung (FPS) zwischen den beiden Geräten, M5Core2 und M5StickC Plus, können aus mehreren Gründen entstehen:

- Prozessorarchitektur und Unterschiede zwischen den ESP32-Chips:
  Beide Geräte verwenden den ESP32-Chip, aber es gibt subtile Unterschiede zwischen den Varianten. Der ESP32 D0WDQ6-V3 im M5Core2 und der ESP32-PICO im M5StickC Plus sind zwar ähnliche Chips, aber der ESP32-PICO ist eine kompaktere Version, die alle notwendigen Komponenten (wie Flash-Speicher, Quarz und Antenne) auf einem einzigen Modul integriert. Dies kann den Stromverbrauch und die Effizienz verbessern, was zu einer besseren Leistung bei Interpolationsaufgaben führen könnte.
- Integriertes Crystal:
  Der ESP32-PICO enthält einen integrierten Quarzkristall, der eine präzise Taktfrequenz bereitstellt. Hier sind einige mögliche Auswirkungen auf die Leistung:
  Ein integriertes Crystal bedeutet, dass die Taktung des Prozessors genau abgestimmt und stabil ist. Es gibt weniger externe Faktoren, die die Taktfrequenz beeinflussen können (wie elektrische Störungen oder Temperaturschwankungen). Diese genaue Taktung könnte dazu führen, dass der ESP32-PICO konstant mit der maximal möglichen Geschwindigkeit arbeitet, was eine höhere Effizienz und bessere Leistung bei Prozessen wie der Bildverarbeitung und Interpolation ermöglicht.
- Integrierter 4 MB Flash-Speicher:
  Der integrierte 4 MB Flash-Speicher des ESP32-PICO kann ebenfalls die Leistung bei Bildverarbeitungsaufgaben erheblich beeinflussen. Hier sind die Gründe, warum dies der Fall sein könnte:
  Da der Flash-Speicher direkt im ESP32-PICO integriert ist, kann die CPU schneller auf den Speicher zugreifen. Im Vergleich zu einem externen Flash-Speicher (wie möglicherweise beim M5Core2), bei dem zusätzliche Latenzen beim Speicherzugriff auftreten können, arbeitet der ESP32-PICO effizienter, da weniger Zeit für den Datenaustausch zwischen CPU und Speicher benötigt wird. Dies ist besonders wichtig, wenn große Datenmengen wie Bilddaten verarbeitet werden müssen.
- Weniger externe Komponenten:
  Da der ESP32-PICO viele seiner Komponenten, einschließlich Crystal und Flash-Speicher, intern integriert hat, gibt es weniger externe Verbindungen und damit weniger Signalverluste oder Latenzen. Externe Verbindungen, wie sie im M5Core2 möglicherweise existieren, könnten die Verarbeitungsgeschwindigkeit beeinträchtigen. Je weniger externe Komponenten ein Gerät hat, desto direkter und schneller kann die CPU auf wichtige Ressourcen wie den Flash-Speicher zugreifen.


7.	Zusammenfassung:

   Im Rahmen dieses Projekts haben wir uns mit der Bildverarbeitung und Interpolation von Thermalkameradaten auf zwei verschiedenen Geräten, dem M5Core2 und dem M5StickC Plus, beschäftigt. Ziel war es, Bilddaten einer THz-Kamera von einer Auflösung von 32x24 Pixeln auf 135x240 Pixel zu interpolieren und verschiedene Interpolationsmethoden zu testen. Zu den angewandten Algorithmen gehörten lineare, bilineare, bicubische und Nearest-Neighbour-Interpolation sowie eine spezielle Interpolation, die wir aus dem Quellcode des M5StickC Plus isolieren konnten.

Um die Effizienz dieser Algorithmen zu evaluieren, haben wir sie auf beiden Geräten implementiert und getestet. Dabei wurden die FPS (Frames per Second) als Leistungskennzahl herangezogen, um die Geschwindigkeit der Bildverarbeitung zu vergleichen. Die Ergebnisse zeigten signifikante Unterschiede zwischen den beiden Geräten, wobei der M5StickC Plus in den meisten Fällen höhere FPS erzielte als der M5Core2.

Die Unterschiede in der Leistung lassen sich vor allem auf hardwaretechnische Faktoren zurückführen. Obwohl auf beiden Geräten die gleiche Software ausgeführt wurde, spielten die Unterschiede in der Prozessorarchitektur, der Taktfrequenzstabilität durch integrierte Quarze und die Effizienz des integrierten Flash-Speichers des ESP32-PICO-D4 eine entscheidende Rolle für die bessere Performance des M5StickC Plus.

Insgesamt konnte durch dieses Projekt gezeigt werden, wie wichtig die Hardwarekonfiguration für die Performance von Bildverarbeitungsalgorithmen ist. Die Ergebnisse liefern wertvolle Erkenntnisse für die weitere Optimierung von Bildverarbeitungsprozessen auf Embedded-Systemen, insbesondere im Hinblick auf Echtzeit-Anwendungen wie die Thz-Video Camera.





