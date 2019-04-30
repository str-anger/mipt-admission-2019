#!/bin/bash

# Название файла в который будут вписываться новые параметры
file_name="some_file"

# Функция вывода сообщений на экран
function start_output()
{
	parameters=$"$@"
	# Создать файл, в который будут передаваться параметры, если он еще не создан
	if [ ! -e "$file_name" ]; then
		touch "$file_name"
	fi		 
	echo $parameters
 	while (true)
	do 
		# Если файл не пустой
		if [ -s "$file_name" ]
		then
			# Считать из него строку с параметрами
			buf=$(cat $file_name)
			# Добавить эту строку к существующим параметрам, которые уже выводятся на экран
			parameters+=$(printf "\n%s" $buf)
			# Очистить файл
			sed -i "/$buf/d" "$file_name"	
		fi	
		sleep 1
		echo $parameters
		done
}

function configure_the_parameters()
{
	parameters=$"$@"
	process_name=$"$0"
	application_name=${process_name:2}	
	process_id=$(pgrep $application_name)
	# Получение PID процессов с именем echoloop.sh
	b=$(echo $process_id | awk '/ /{print $0}')
	$ Если один процесс уже запущен
	if [ -n "$b" ]
	then
		echo "echoloop for $parameters finished"
		[ -n "$1" ] 
			# Передать параметры в файл, из которого первый процесс считает параметры
			for script in "$*"; do
				echo -e $script >> $file_name
			done
		exit 0
	# Если процесс пока не был запущен
	else
		# Проверяем переданные параметры
		if [ -n "$parameters" ]
		then	
			start_output $parameters
		else
			echo "No parameters found!"
		fi
	fi
}

configure_the_parameters $@