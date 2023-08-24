a=0

while [ $a -lt 10 ]
do
	dir=week${a}
	mkdir $dir
	cd $dir
	mkdir pset
	mkdir lab
	cd ..
	a=`expr $a + 1`
done
 
