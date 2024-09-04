# Server grouping optimization problem (Minimum Makespan Scheduling)

## Документация

## Примечания Разработчика
В этом репозитории вы можете найти софт разработанный мной в процессе изучения проблемы оптимизации эффективного пространства распределённого хранилища данных при использовании разных подходов кодирования (Репликация или помехоустойчивый код Рида-Соломона). Все ссылки на исследования и статьи, использованные в моём исследовании можно найти в **Приложении A**.

### Использование K реплик в качестве схемы хранения. Область отказа - серверная стойка.
Первым я рассмотрел случай Репликации, где за область отказа была взята серверная стойка. В таком случае главным моим вопросом было -- как разместить **N** серверов различной ёмкости по **K** стойкам так, что бы максимизировать эффективное пространство. Первым мне в голову пришёл жадный алгоритм:

#### Алгоритм:
1) Сортируем сервера по убыванию ёмкости.
2) Выбираем стойку с самым маленьким объёмом.
3) Выбираем сервер (из списка нераспределённых) с самом большой ёмкостью и добавляем в эту стойку.
4) Удаляем сервер из списка нераспределённых серверов.
5) Если неравпределённые сервера закончились - заканчиваем, иначе переходим к шагу (2).

#### Асимптотика:
Таким образом за Используя в качестве алгоритма сортировки std::sort (O(NlogN)) и бинарное дерево (можно кучу, просто дерево есть в stl (std::map)) для хранения стоек и нахождения самой незаполненной получаем итоговую асимптотику O(N logN).

Однако, как показала практика данный алгоритм не даёт точного ответа. Ответ очень близок к идеальному, однако имеется погрешность. На этом этапе я решил посёрфить интеренет в поисках похожих проблем. На тот момент я нашёл 2 статьи которые натолкнули меня на мысли о Динамическом Программировании и Бинарном Поиске. Это Задача о поиске подпоследовательностей с целью минимизации максимального элемента и Задача о поиске подмножеств чисел с целью максимизации суммы максимальных и минимальных элементов. Однако дальнейшие рассуждения в эту сторону ни к чему не привели. Я опять отправился исследовать интернет, и в это раз наткнулся на проблему **Minimum Makespan Scheduling (Далее MMS)**. Она формулируется так:
Дано **N** задач и **K** полностью идентичных машин. Известно, что **i-я** задача выполняется на машине за **P_i** секунд. Вопрос: как расределить задачи по машинам так, что бы все задачи выполнились как можно раньше, т.е. что бы минимизировать время работы самой загруженной машины. 

Бинго! Как можно заметить задача крайне похожа на нашу проблему. Судя по статье, данная задача является NP-hard. Остался вопрос, можно ли свести одну задачу к другой. Ответ - Да! Можно заметить, что решение нашей задачи так же является решением задачи MMS. Небольшое доказательство данного факта можно найти в **Приложении B**. Тогда получается, что наша задача так же NP-hard (иначе получится, что решив нашу задачу, мы решим ещё и MMS). В той же статье я ещё и нашёл придуманный мной, или уже правильней сказать, перепредуманный, Жадный алгорит (он оказался ещё и одним из самых эффективных).

### Приложение A:
* Задача о подпоследовательностях - https://www.geeksforgeeks.org/split-the-given-array-into-k-sub-arrays-such-that-maximum-sum-of-all-sub-arrays-is-minimum/ 
* Задача о подмножествах - https://www.geeksforgeeks.org/split-array-into-k-subsets-to-maximize-their-sum-of-maximums-and-minimums/
* Статья про Minimum Makespan Scheduling - https://www.cs.cmu.edu/afs/cs/academic/class/15854-f05/www/scribe/lec03.pdf

### Приложение B:
Доказательство сведения задачи MMS к нашей. В нашем случае достаточно показать, что ответ к нашей проблеме является ответом к MMS. Пусть это не так, тогда в решении нашей проблемы, можно уменьшить нагрузку на одну машину так, что в итоге максимум по всем машинам уменьшится. В рамках нашей задачи это значит, что можно уменьшить объём одной стойки так, что максимум по всем стойкам станет меньше. Давайте тогда убранную память положим в наименьшую стойку => минимум по всем стойкам увеличится => нашли более оптимальное решение. Противоречие.