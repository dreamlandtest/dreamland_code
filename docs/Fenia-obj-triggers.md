# Триггера для предметов

## Общие для всех видов предметов

```onGet(ch)``` - предмет попал в инвентарь к ch

```onDrop(ch)``` - ch уронил предмет. Надо вернуть true, если в триггере предмет разрушен, или если хотим чтобы он не утонул в воде и не рассыпался (при extra_flag melt_drop).

```onSac(ch)``` - ch принес в жертву предмет. Вызывается сразу после сообщения "Такой-то приносит в жертву богам что-то.". Если уничтожаем предмет, надо вернуть true.

```onEntry()``` - тот, кто нес предмет, вошел в какую-то комнату.

```onGive(ch, vict)``` - ch дал предмет чару vict. Если вернем true, не будет вызван триггер onGive у vict, и onGet(vict) у этого предмета. True нужно возвращать, если предмет уничтожен или не задержался в руках у vict.

```onGreet(ch)``` - ch вошел в комнату, а предмет - в руках у стоящего в комнате или лежит на полу.

```onFight(ch)``` - ch, несущий предмет, сражается. Вызывается каждый баттл-раунд (раз в 4 секунды).

```onDeath(ch)``` - ch, на которого одет предмет, пытается умереть. Если возращаем true, он воскресает и остается стоять.

```onSpeech(ch, str)```- ch произнес фразу 'str' стоя рядом с предметом

```onArea()``` - вызывается каждую минуту. Если разрушаем предмет, или не хотим, чтобы для него тикал timer, надо вернуть true.

```onExtract(count)``` - предмет разрушается. count == false означает, что разрушение не уменьшит счетчика предметов с этим внумом. Пример - выход чара из мира. На момент вызова триггера предмет уже удален из всех списков.

```onQuit(ch,count)``` - ch покидает мир. Если разрушаем предмет, надо вернуть true. count == false при обычном quit, и true при делите.

```onExamine(ch)``` - ch пялится на предмет (команда examine). Если вернем true, стандартные сообщения от examine не появятся.

```onInvisible(ch)``` - Если вернем true, предмет будет всегда невидим для ch. Если false - будут вызваны стандартные проверки видимости-невидимости.

```onPut(ch,obj,cont)``` - ch кладет obj в контейнер cont. Триггер вызывается для всех троих.

```onUse(ch,str)``` - ch применил на предмет команду 'use' с аргументами str. Если вернуть false, ch будет 'вертеть в руках, не зная что с ним делать'.

```onExtraDescr(ch,str)``` - возвращает строку, которую надо показать чару ch, при взгляде на экстра-дескр str. Если вернуть пустую строку, продолжится поиск другого экстра-деска, иначе -- покажет то что мы вернули, и оборвет поиск.

```onLook(ch, str)``` - на предмете был экстра-дескр с ключевым словом str, и ch только что увидел его (например, предмет - это книга, и ch выполнил "look page1", тогда в onLook передастся ch и "page1").

```onCommand(ch, cmdName, cmdArgs)``` - ch, держащий этот предмет в руках, пытается выполнить команду с именем cmdName и аргументами cmdArgs. если триггер возвращает true, команду выполнить не получится. осторожно, с помощью этого триггера можно полностью обездвижить персонажа, если всегда возвращать true!

```onPourOut(ch, container, liqname, amount)``` - персонаж ch вылил из сосуда container жидкость под названием liqname в количестве amount, облив этой жидкостью того, кто несет этот предмет.

## Триггеры для экипировки (броня, одежда и так далее)

```onWear(ch)``` - ch одел предмет на тело

```onRemove(ch)``` - ch снял предмет с тела

```onCantEquip(ch)``` - может ли ch надеть этот предмет на себя. Если вернуть true, то надеть не получится.

```onCanDress(ch, victim)``` - может ли персонаж ch надеть этот предмет на другого персонажа victim или на себя (victim в этом случае будет равен ch). Если вернуть true, то получится надеть, по умолчанию одевать других нельзя.

```onDress(ch, victim)``` - персонаж ch только что надел этот предмет на victim. 

## Триггеры для контейнеров (container)

```onFetch(ch,obj)``` - ch вынул obj из этого предмета-контейнера

```onClose(ch)``` - ch закрыл этот предмет-контейнер. Вызывается сразу после фразы "Ты закрываешь <что-то>".

```onOpen(ch)``` - ch открыл этот предмет-контейнер. Вызывается сразу после фразы "Ты открываешь <что-то>".

```onCantFetch(ch, obj, pocket)``` - чар ch пытается вытащить предмет obj из этого контейнера (возможно из кармана с именем pocket). Если вернуть true - вытащить не получится.

```onCantPut(ch, obj, pocket, verbose)``` - чар ch пытается положить предмет obj в этот контейнер (возможно в карман с именем pocket). Если вернуть true - положить не получится. Если параметр verbose равен true, нужно вывести персонажу сообщения о неудаче, если же false - можно ничего не сообщать.

```onPut(ch,obj,cont)``` - ch кладет obj в контейнер cont. Триггер вызывается для всех троих.

## Триггеры для мебели (furniture)

```onShowWhere(ch, looker)``` - пусть ch сидит на предмете, а другой чар (looker) выполняет look. если onShowWhere вернет строку, то looker увидит ее вместо стандартного "на стуле", "в кровати" и тп.

```onShowEnd(ch, looker)``` - пусть ch сидит на предмете, а другой чар looker выполняет look. строку, возвращаемую onShowEnd, looker увидит в конце стандартных сообщений (например "такой-то сидит на стуле ... закинув ногу на ногу").

## Триггеры для порталов (portal)

```onPortalLocation(ch)``` - пусть ch пытается войти в портал. если триггер вернет 0, чар попадает в комнату предусмотренную этим порталом, если вернуть число - чар попадет в комнату с указанным внумом.

## Триггеры для пищи (food, drink)

```onEat(ch)``` - принимает в параметры чара-едока, если ты изнутри уничтожаешь объект или же наоборот - не хочешь, чтобы он исчез как обычно бывает после 'eat', возвращай true.

```onDrink(ch, liqName, amount)``` - чар ch пьет жидкость liqName в размере глотка amount. При питье из бочонков он равен полю sip_size у жидкости, а из фонтанов - в три раза больше. 

```onEmpty(ch, liqname, old_amount)``` - персонаж ch опорожнил этот предмет, вылив из него жидкость под названием liqname в количестве old_amount. Если вернуть true, не будет никаких сообщений по умолчанию и лужи.

