
i=0

D="CZE"
N="000"
PREFIX=""

while [ -f "${D}/${N}${i}_in.txt" ]
do
	./a.out < "${D}/${N}${i}_in.txt" > "${D}/${i}.ref"
	
	if [ $i -lt 4 ] && ! diff "${D}/${i}.ref" "${D}/${N}${i}_out.txt" > /dev/null; then
		printf '\nfile number: %s\n' $i
		printf '\ndiff output:\n'
		diff -y "${D}/${N}${i}_out.txt" "${D}/${i}.ref"
		printf "\ninput:\n"
		cat "${D}/${N}${i}_in.txt" -n
		printf "\n"
		break
	fi

	i=$((i + 1))
	if (("$i" == 10)); then
		N="00"
	fi
done
