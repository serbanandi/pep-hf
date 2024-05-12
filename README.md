# Párhuzamos és eseményvezérelt programozás beágyazott rendszereken (VIMIAC08) házi feladat
## Banánfa - linuxos vezérléssel

### A játék célja

A feladat minél több lehulló gyümölcs begyűjtése négy banánfáról. A banánfákat elhagyó gyümölcsök az alsó (alfanumerikus) kijelző négy mezeje valamelyikén eshetnek le. Ezeket alul egy kosár segítségével kell elkapni.

### A játék indítása

A játék elején meg kell adnunk a játékos nevét. Ezután lehet állítani a potyogás sebességét a W és S gombok segítségével, vagy egy 1-8 közötti szám megadásával. A játék a szóköz lenyomásával indul.

### A játék menete

Ha az egyik banánfán egy gyümölcs már nagyon érett, akkor ezt a hozzá tartozó mező legfelső vízszintes szegmensének kigyújtásával jelezzétek. Ezek után a banán egy meghatározott sebességgel elkezd lefele esni az adott mező középső függőleges szegmensein (értelem szerűen először a felső szegmenset éri el, majd az alsót). Ha a banán földet érésekor alatta volt a kosár, akkor sikerült elkapni. A kosarat a pozíciójának megfelelő mező alsó vízszintes szegmense jelképezi. Mozgatni pedig az A és D billentyűkkel lehet balra ill. jobbra. Összesen 25 banán elkapására van lehetőség, utána vége van a játéknak. Az eddig lehullt illetve az elkapott banánok számát a felső (numerikus) kijelző egyik illetve másik oldali két-két digitjén jelezzük (középen a kettősponttal elválasztva).

### A játék vége

A játék 25 banán leesése után véget ér. Ezt követően megjelenik a számítógépen a nehézséggel súlyozott pontszámunk, amit - amennyiben ez rekord - elmenthetünk a dicsőségtáblára.


![image](https://github.com/serbanandi/bambi-hf/blob/7637c89ccdda8b4027ead1a4ff82fa68085d2d30/spec-img.png)