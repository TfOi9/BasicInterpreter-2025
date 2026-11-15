# BASIC Interpreter Extended

## BASIC 语言

一个 BASIC 程序由一系列带有标号的语句组成，一个简单的例子如下：

```BASIC
10 REM A + B Problem
20 INPUT a
30 INPUT b
40 LET sum = a + b
50 PRINT sum
60 END
```

### 词法

BASIC 语言的标识符由一个或者多个字母（或数字）组成，且不为保留字。在本次作业中，我们认为标识符是大小写敏感的，举例而言，`ID` `id` `Id` `iD` 被认为是四个不同的标识符。

### 行号

每个语句开头的数字标号代表语句的行号，它是一个非负整数。一个程序的语句由行号从小到大依次执行。相邻语句的行号不一定需要是连续的。中间有跳跃的行号可以使我们方便地在语句之间插入新的语句，例如：

```BASIC
20 INPUT a
30 INPUT b
40 LET sum = a + b
```

假如我们还想输入第三个变量 `c`，并且求算三个变量的和，我们可以在行号 30 与 40 之间插入新的一行：

```BASIC
20 INPUT a
30 INPUT b
35 INPUT c
40 LET sum = a + b + c
```

### 顺序语句

#### REM：注释
REM 用于表示一行注释；REM 语句不会被执行。

#### DIM：定义
DIM 的语法形如 `DIM var = exp`，用于定义一个变量。

#### LET：赋值
LET 的语法形如 `LET var = exp`，用于给一个变量赋值。

#### PRINT：输出
PRINT 的语法形如 `PRINT exp`，用于打印某个表达式的值，并换行。

#### INPUT：输入
INPUT 的语法形如 `INPUT var`，用于输入某个变量。

#### END：结束
执行到 END 时，程序结束。

### 数据类型

BASIC 的数据类型有如下五种，可以使用符号后缀指明，用法可以参考下述的例子。

| 类型 | 意义（与 C++ 对应） | 后缀 | 示例 |
| :-: | :-: | :-: | :-: |
| INTEGER | `int32_t` | `%` | `COUNT%` |
| LONG | `int64_t` | `&` | `SIZE&` |
| SINGLE | `float` | `!` | `PRICE!` |
| DOUBLE | `double` | `#` | `VALUE#` |
| STRING | `std::string` | `$` | `NAME$` | 

数据类型可以不被指明，此时需要自动推断：不含小数点的数推断为 INTEGER，含小数点的数推断为 SINGLE；用双引号包裹的字符串推断为 STRING。

如果未指明类型，也无法自动推断，那么类型默认为 SINGLE。

举例而言，我们可以写出以下的语句：

```BASIC
10 DIM X = 10
20 REM X is considered as type INTEGER.
30 LET Y& = 1000000000000
40 DIM PI# = 3.141592653589793
50 LET NAME$ = "Alice"
```

此外，我们也可以使用显式声明来定义变量：

```BASIC
10 DIM age AS INTEGER
20 DIM price AS SINGLE
30 DIM id AS STRING
```

#### 类型转换函数

| 函数名 | 返回值类型 |
| :-: | :-: |
| CINT | INTEGER |
| CLNG | LONG |
| CSNG | SINGLE |
| CDBL | DOUBLE |

其行为可以等同于 C++ 中 `static_cast` 的行为。

### 数组

BASIC 数组分为定长与可变长两种，且有高维数组。

#### 定长数组

可以使用以下语法来定义一个定长的数组：

```BASIC
DIM arr(10)
DIM values#(20)
DIM matrix(3, 4) AS INTEGER
```

其中，`matrix` 是一个 3 * 4 的二维数组。

在本次作业中，只需要实现一维和二维定长数组。

#### 可变长数组

可以使用以下语法来定义和操作一个可变长数组：

```BASIC
10 DIM vec() AS INTEGER
20 REDIM vec(10)
30 REM vec now has size 10
35 LET vec(1) = 5
40 REDIM vec(20)
45 REM vec's value will be lost if redimed
50 LET vec(1) = 3
60 REDIM PRESERVE vec(40)
65 REM vec's value will be preserved
70 ERASE vec
75 REM vec is freed
```

我们只需要考虑一维可变长数组。

### 算术表达式

我们需要实现如下的各种运算：

| 运算符 | 详细解释 |
| :-: | :-: |
| `+` | 加法或字符串连接 |
| `-` | 减法或取负 |
| `*` | 乘法 |
| `/` | 浮点除法 |
| `\` | 整数除法 |
| `MOD` | 取模 |
| `^` | 幂运算 |

为了简化，我们约定参与运算的两个运算符必须是同一类型的，如果类型不同则需要手动使用转换函数，否则应当报错。

特别地，浮点除法的两个参数如果都是整数类型的，返回 SINGLE 类型的商；整除和取模运算符仅限整数类型参与运算。

除了整数参与的浮点除法，返回值应当与运算数同类型。

运算符的优先级为括号 > 幂运算 > 乘除、取模 > 加减。

### 逻辑表达式

我们需要实现以下的比较运算符：

| 运算符 | 详细解释 |
| :-: | :-: |
| `=` | 相等 |
| `<>` | 不等 |
| `>` | 大于 |
| `<` | 小于 |
| `>=` | 大于等于 |
| `<=` | 小于等于 |

其中，比较的两个运算数必须同类型；字符串的大小比较遵循字典序比较。

我们需要实现以下的逻辑连接词：

| 连接词 | 详细解释 |
| :-: | :-: |
| AND | 两个条件同时满足 |
| OR | 两个条件满足一个 |
| NOT | 取反 |

优先级为 NOT > AND > OR.

### 分支语句

我们实现 IF 语句，语法如下：

```BASIC
10 INPUT score
20 IF score >= 95 THEN
25  PRINT "4.3"
30 ELSEIF score >= 90 THEN
35  PRINT "4.0"
40 ELSE
45  PRINT "<4.0"
50 END IF
```

其中，IF 语句可以有任意多个 ELSEIF 子句，可以有至多一个 ELSE 子句，且要用 END IF 结尾。

进入内层语句块时，会进入内层变量作用域，其概念与特性与 C++ 的相同，FOR, WHILE 循环也类似。

Dijkstra 曾经说过，GOTO 是有害的，所以我们把它删了。

### 循环语句

#### FOR 循环

```BASIC
10 FOR I = 1 TO 10
20  PRINT I
30 NEXT I
```
注意循环包含了 10.

```BASIC
10 FOR I = 10 TO 1 STEP -1
20  PRINT I
30 NEXT I
```

可以指定步长。

```BASIC
10 FOR I = 1 TO 100
20  IF I = 20 THEN
30   EXIT FOR
40  END IF
50 NEXT I
```

可以提前退出。

#### WHILE 循环

```BASIC
10 LET X% = 0
20 WHILE X < 10
30  X = X + 1
40  PRINT X
50 WEND
```

## BASIC 解释器

### 程序

向 BASIC 解释器中写入一行带有行号的语句表示向解释器中添加这一行。

写一个行号与之前重复的语句将会覆盖对应行。

写一个行号，后面留空并换行表示删除对应行号的语句（如果有）。

写一个无行号的语句表示立即执行本语句。

### 指令

| 指令 | 作用 |
| :-: | :-: |
| RUN | 从最小行号开始执行 |
| LIST | 按行号顺序列出所有语句 |
| CLEAR | 清空所有语句和变量状态 |
| QUIT | 退出解释器 |
| HELP | 打印帮助信息（评测不会涉及） |

### 报错

| 错误信息 | 含义 |
| :-: | :-: |
| `DIVIDE BY ZERO` | 除零错误 |
| `INVALID NUMBER` | 数字格式错误 |
| `VARIABLE NOT DEFINED` | 变量未定义 |
| `SYNTAX ERROR` | 其他语法错误 |

## 评测标准

| 内容 | 分数占比 |
| :-: | :-: |
| 解释器 | 80% |
| Bonus | 至多 20% |
| Code Review | 20% |

共有 400 个测试点，按照通过数目比例给分。

## Bonus

- STRING 处理函数，3%
- SWITCH 语句，3%
- DO 循环，3%
- SUB 过程，5%
- FUNCTION 函数，5%
- ON ERROR 错误处理，5%
- TYPE 自定义数据类型，5%

详细的 Bonus 待补全。
