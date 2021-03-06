tic-tac-toe
====

Жуковский Анатолий, группа 795 ~ Zhukovskiy.AM@phystech.edu

Условие
----

**Крестики-нолики.** Оцените как можно точнее число возможных полей, возникающих в игре крестики-нолики `3×3`. Приведите решение.

Описание решения
----

В решении я привожу `2` оценки числа возможных полей: `138` и `764`. Обе оценки получены при рассмотрении только состояний поля после учета симметрии поля (то есть поворотов и отражений). В первом случае учитывается `138` терминальных позиций (т.е. тех в которых наступает победа одного из игроков либо ничья). Во втором случае учитывается `764` терминальных и промежуточных позиций, без учёта пустого поля. 

Проверка решения
----

Решение написано и протестировано на следующей системе:
- `conda 4.6.14`
- `Python 3.6.8 :: Anaconda, Inc.`

Для удобства решение представлено в 3-х форматах: `.ipynb`, `.html`, `.pdf`:
- [solution.ipynb](solution.ipynb)
- [solution.html](solution.html)
- [solution.pdf](solution.pdf)
