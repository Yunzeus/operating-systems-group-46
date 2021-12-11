# group46-shell-in-c
Linux Shell program in C

Writed by Mert Can Yılmaz g191210018 and group 46

Alihan Sarac g191210057

Yunus Akgul g191210003

Salih Yener g191210036


# Design overview

46mcshell dosyasında bulunan fonksiyonların hangi işlevleri yaptığı ve 46mcshell.c dosyası içinde yorum satırları halide bulunmakta.

feaof ile prompt üzerindeki değişiklikler sürekli dinleniyor fgets ile okunuyor. Sonrasında parsa işlemi yapılıp boşluklara göre argüman parçalarına ayrılıyor. Eğer il argüman parçası built-in komutuysa:

cd : Promın çalıştığı dizini değiştiriyor. setenv fonkisyonu ile pwd değeri değiştiriliyor.

showpid : built-in olmayan komutları çalıştırmak için her fork kullanıldığında oluşan child'ın process idsi, gerçekleşen son 5 çocuk prosesi tutan diziye baştan ekleniyor. Bu komut sadece bu diziyi yazdırmakla görevli. 

exit: Programdan çıkıp exit yazdırıyor.

Eğer bu komutlardan biri girilmemişse pid_exec fonkisiyonu cağrılıyor, fork ile child process oluşturuluyor. Env değeri ayarlanıyor. Hata varsa handle ediliyor. execvp ile program çalıştırılıp parametreleri yüklenip kapatılıncaya kadar waitpid ile bekleniyor


# Running the program

Program linux ortamında hazılanmış ve derlenmiştir. İşletim sistemi kurulduktan sonra hazır bulunan paketler ödevde kullanılmıştır. başka işletim sisteminde bu paketler bulunmayacağından ekstaradan sisteme yüklemek gerekebilir.

Eğer sisteminizde make yüklüyse proje dosyalarını indirdiğiniz dizine girip:

make compile  ile kodu derleyebilir

make run   ile derlenen programı çalıştırabilirsiniz

eğer değilse

gcc -o 46mcshell 46mcsheel.c   komutu ile derleyebilir

./46mcsheell diyerek çalıştırabilirsiniz.

# Running the program in batch mode 

Örnek kullanım

![execute](https://raw.githubusercontent.com/DEONSKY/operating-systems-group-46/main/Ekran%20Görüntüsü%20(91).png)

# knowhow

Ödev dökümantasyonun takılacağımız noktları aşabilmemiz için keywordlar verilmişti. Bu belli bir oranda işimizi kolaylaştırdı. Terminalden okuma kısmı uğraştıcıydı. Ama internetten benzer örneklere bakarak bir yol bulmayı başardık. showpid fonksiyonunda dökümantasyon biraz kafa karıştırıcı oldu. En az 5 child process'i yazdırmamız isteniyordu. Ama program ilk açıldığında hiç child process oluşturulmadığı boş kalması gerekiyor. Child process başlatıp dosya yükleme ve onun parametrelerini yükleme kısmı da biraz uğraştırdı

# references

https://stackoverflow.com/

https:/www.github.com/

https:/www.youtube.com/

https://www.geeksforgeeks.org/


