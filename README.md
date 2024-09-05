# Server grouping optimization problem (Minimum Makespan Scheduling)

Написал и разработал Kruzenger (https://github.com/Kruzenger). 

## Примечания Разработчика
В этом репозитории вы можете найти софт разработанный мной в процессе изучения проблемы оптимизации эффективного пространства распределённого хранилища данных при использовании разных подходов кодирования (Репликация или помехоустойчивый код Рида-Соломона). Все ссылки на исследования и статьи, использованные в моём исследовании можно найти в **Приложении A**.

## Документация
### Установка:
Для работы с данной библиотекой вам может понадобиться установить дополнительный софт, а именно ```<nlohmann/json.hpp>``` для работы с JSON файлами.
Для подключения этой библиотеки просто добавьте в ваш проект ```#include "your_path_to_lib/src/server_infrastructure_utils.h"``` 

### Формат ввода данных
Данный софт может считать данные из терминала и файла JSON. 
Для чтения из файла JSON в метод ```JsonServersParser::Parse()``` нужно передать путь, относительно корня проекта до файла. По умолчанию используется ```./input.json```.  Для файла JSON формат данных таков:

```json
{
  "Servers": [
    {
      "name": "Server_1",
      "size": "100 TB"
    },
    {
      "name": "Server_2",
      "size": "50 TB"
    },
    {
      "name": "Server_3",
      "size": "20 TB"
    },
    {
      "name": "Server_4",
      "size": "40 TB"
    },
    {
      "name": "Server_5",
      "size": "90 TB"
    }
  ]
}
```

Для считывания с терминала формат данных таков:

```
N //количество серверов
A_i B_i // A_i - имя сервера, B_i - размер
```

### API
Весь функционал лежит в ``` namespace server_infrastructure_utils ```.
Пример использования библиотеки для группировки серверов заданных через JSON.

Метод ```GreedyGrouper::Group()``` для вызова жадного алгоритма группировки является статическим и принимает 2 параметра - ```ClasterData``` с информацией о серверах в кластере и ``` int16_t number_of_replicas ``` с информацией о количестве репликаций. Вы можете расширять библиотеку и добавлять свои методы группировки, аналогичным образом.

```cpp
#include "./server_infrastructure_utils.h"

int main() {
  int16_t number_of_cores = 5;
  server_infrastructure_utils::GroupedServerCores cores =
      server_infrastructure_utils::GreedyGrouper::Group(
          server_infrastructure_utils::JsonServersParser::Parse(),
          number_of_cores);

  return 0;
}
```

### О структурах данных
В данной библиотеке используются 3 структуры данных:

```cpp
struct ServerData {
  std::string name;
  int64_t size;
};
```

```cpp
struct ClasterData {
  std::vector<ServerData> servers;
};
```

```cpp
struct GroupedServerCores {
  std::vector<std::vector<ServerData>> cores;
};
```

## Использование K реплик в качестве схемы хранения. Область отказа - серверная стойка.
Первым я рассмотрел случай Репликации, где за область отказа была взята серверная стойка. В таком случае главным моим вопросом было -- как разместить **N** серверов различной ёмкости по **K** стойкам так, что бы максимизировать эффективное пространство. Первым мне в голову пришёл жадный алгоритм:

### Алгоритм:
1) Сортируем сервера по убыванию ёмкости.
2) Выбираем стойку с самым маленьким объёмом.
3) Выбираем сервер (из списка нераспределённых) с самой большой ёмкостью и добавляем в эту стойку.
4) Удаляем сервер из списка нераспределённых серверов.
5) Если нераспределённые сервера закончились - прекращаем работу, иначе переходим к шагу (2).

### Асимптотика:
Таким образом используя в качестве алгоритма сортировки std::sort (O(NlogN)) и бинарное дерево (можно кучу, просто дерево есть в stl (std::map)) для хранения стоек и нахождения самой незаполненной получаем итоговую асимптотику O(N logN).

### Особенности алгоритма и дальнейшие исследования.
Однако, как показала практика данный алгоритм не даёт точного ответа. Ответ очень близок к идеальному, однако имеется погрешность. На этом этапе я решил посёрфить интернет в поисках похожих проблем. На тот момент я нашёл 2 статьи, которые натолкнули меня на мысли о Динамическом Программировании и Бинарном Поиске. Это Задача о поиске подпоследовательностей с целью минимизации максимального элемента и Задача о поиске подмножеств чисел с целью максимизации суммы максимальных и минимальных элементов. Однако дальнейшие рассуждения в эту сторону ни к чему не привели. Я опять отправился исследовать интернет, и в это раз наткнулся на проблему **Minimum Makespan Scheduling (Далее MMS)**. Она формулируется так:
Дано **N** задач и **K** полностью идентичных машин. Известно, что **i-я** задача выполняется на машине за **P_i** секунд. Вопрос: как распределить задачи по машинам так, что бы все задачи выполнились как можно раньше, т.е. что бы минимизировать время работы самой загруженной машины. 

Бинго! Как можно заметить задача крайне похожа на нашу проблему. Судя по статье, данная задача является NP-hard. Остался вопрос, можно ли свести одну задачу к другой. Ответ - Да! Можно заметить, что решение нашей задачи так же является решением задачи MMS. Небольшое доказательство данного факта можно найти в **Приложении B**. Тогда получается, что наша задача так же NP-hard (иначе получится, что решив нашу задачу, мы решим ещё и MMS). В той же статье я ещё и нашёл придуманный мной, или уже правильней сказать, переизобретённый, Жадный алгоритм (он оказался ещё и одним из самых эффективных).

## Использование Избыточного кодирования N+M в качестве схемы хранения. Область отказа - серверная стойка.
В данном случае в качестве кодирования используется помехоустойчивый код Рида-Соломона. При кодировании **N+M** мы будем разбивать данные на **N** блоков и добавлять **M** блоков избыточного кодирования. В таком случае у нас будет использоваться **N + M** серверных стоек, причём мы можем восстановить данные и не потерять доступ к ним, при потере до **M** стоек. Получается для оптимизации используемого пространства нам нужно равномерно распределить сервера по **N + M** стоек. Т.е. для нас, как разработчиков данная задача сводится к предыдущей задаче оптимизации для **K** репликаций, только вместо **К** в качестве параметра нужно передавать **N + M**.

## Приложение A:
* Задача о подпоследовательностях - https://www.geeksforgeeks.org/split-the-given-array-into-k-sub-arrays-such-that-maximum-sum-of-all-sub-arrays-is-minimum/ 
* Задача о подмножествах - https://www.geeksforgeeks.org/split-array-into-k-subsets-to-maximize-their-sum-of-maximums-and-minimums/
* Статья про Minimum Makespan Scheduling - https://www.cs.cmu.edu/afs/cs/academic/class/15854-f05/www/scribe/lec03.pdf
* Про код Рида-Соломона https://habr.com/ru/articles/538870/

## Приложение B:
Доказательство сведения задачи MMS к нашей. В нашем случае достаточно показать, что ответ к нашей проблеме является ответом к MMS. Пусть это не так, тогда в решении нашей проблемы, можно уменьшить нагрузку на одну машину так, что в итоге максимум по всем машинам уменьшится. В рамках нашей задачи это значит, что можно уменьшить объём одной стойки так, что максимум по всем стойкам станет меньше. Давайте тогда убранную память положим в наименьшую стойку => минимум по всем стойкам увеличится => нашли более оптимальное решение. Противоречие.

Альтернативно можно рассмотреть дополнения до объёма каждого диска (X = -Y) со смещением на модуль объёма самого болього диска. Тогда буквально задача сводится к MMS.