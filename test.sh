cd OS_PJ1_Test/
for dat in $(ls RR*);
do
    echo $dat
    ../main < $dat > ../out/$dat
done
