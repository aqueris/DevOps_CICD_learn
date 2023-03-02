## Part 1. Готовый докер

* Поднять виртуальную машину Ubuntu Server 20.04 LTS

* Скачать и установить на виртуальную машину gitlab-runner

![](./screenshots/1.1.png "Download and install gitlab-runner on the virtual machine")

* Запустить gitlab-runner и зарегистрировать его для использования в текущем проекте (DO6_CICD)

![](./screenshots/1.2.png "Run gitlab-runner and register it for use in the current project (DO6_CICD)")

## Part 2. Сборка

* Написать этап для CI по сборке приложений из проекта C2_SimpleBashScripts:

* В файле gitlab-ci.yml добавить этап запуска сборки через мейк файл из проекта C2

* Файлы, полученные после сборки (артефакты), сохранять в произвольную директорию со сроком хранения 30 дней.

![](./screenshots/2.png "Write a stage for CI to build applications from the C2_SimpleBashScripts project:")

## Part 3. Тест кодстайла

* Написать этап для CI, который запускает скрипт кодстайла (clang-format):

![](./screenshots/3.1.png "Write a stage for CI that runs a codestyle script (clang-format):")

* Если кодстайл не прошел, то "зафейлить" пайплайн

* p.s. Mой кодстайл прошел, ошибок не было

![](./screenshots/3.2.png "If the codefile didn't pass, ''fail'' the pipeline")

* В пайплайне отобразить вывод утилиты clang-format

![](./screenshots/3.4.png "In the pipeline, display the output of the clang-format utility")

## Part 4. Интеграционные тесты

* Написать этап для CI, который запускает ваши интеграционные тесты из того же проекта:

* Запускать этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно

![](./screenshots/4.1.png "Write a stage for CI that runs your integration tests from the same project:")

* Если тесты не прошли, то "зафейлить" пайплайн

* В пайплайне отобразить вывод, что интеграционные тесты успешно прошли / провалились

![](./screenshots/4.2.png "In the pipeline, display the output of the succeeded / failed integration tests")

## Part 5. Этап деплоя

* Поднять вторую виртуальную машину Ubuntu Server 20.04 LTS

![](./screenshots/5.3.png "Start the second virtual machine Ubuntu Server 20.04 LTS")

* На машине 1:
   * Включить дополнительный адаптер внутренняя сеть
   * Задать ip 10.12.96.0 машины 1, отредактировав конфиг netplan
   * установить expect - printf "student\n" | sudo -S apt install expect -y

* На машине 2:
   * Включить дополнительный адаптер внутренняя сеть
   * Задать ip 192.168.1.2 машины 2, отредактировав конфиг netplan
   * дать права в том числе на запись - sudo chmod +x /usr/local/bin/

* Написать этап для CD, который "разворачивает" проект на другой виртуальной машине:

* Запускать этот этап вручную при условии, что все предыдущие этапы прошли успешно

![](./screenshots/5.2.png "Write a stage for CD that ''deploys'' the project on another virtual machine and run this stage manually, if all the previous stages have passed successfully")

* Написать bash-скрипт, который при помощи ssh и scp копирует файлы, полученные после сборки (артефакты), в директорию /usr/local/bin второй виртуальной машины

![](./screenshots/5.4.png "Write a bash script which copies the files received after the building (artifacts) into the /usr/local/bin directory of the second virtual machine using ssh and scp")

* В файле gitlab-ci.yml добавить этап запуска написанного скрипта

* В случае ошибки "зафейлить" пайплайн

![](./screenshots/5.1.png "Successed stage message in pipeline output and check if files exist")

* Сохранить дампы образов виртуальных машин

## Part 6. Дополнительно. Уведомления

* Настроить уведомления о успешном/неуспешном выполнении пайплайна через бота с именем "[ваш nickname] DO6 CI/CD" в Telegram

* Текст уведомления должен содержать информацию об успешности прохождения как этапа CI, так и этапа CD.

* В остальном текст уведомления может быть произвольным.

![](./screenshots/6.1.png "The text of the notification must contain information on the successful passing of both CI and CD stages.")

![](./screenshots/6.2.png "Successed pipeline with notification system")
