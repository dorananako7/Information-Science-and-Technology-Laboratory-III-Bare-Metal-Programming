# デフォルトターゲット（誤爆防止と利便性のため）
all: 23.img

# 疑似ターゲットの設定
.PHONY: all clean

# 任意の.elf から 任意の.img を作る手順
# $< は必要なファイル群の先頭、 $@ はターゲット名
%.img: %.elf
	arm-none-eabi-objcopy -O binary $< $@

# 必要な.o を結合して 任意の.elf を作る手順
# $+ は必要なソースファイル全体 (init.o と %.o) を表します
%.elf: init.o %.o
	arm-none-eabi-ld -m armelf $+ -o $@

# 任意の.cを任意の.oに変換する手順
%.o: %.c const.h
	arm-none-eabi-gcc -c $< -o $@

# 削除されてしまった init.o を init.s から自動で復活させるための手順
%.o: %.s
	arm-none-eabi-as $< -o $@

# 中間ファイルと生成物をすべて削除する手順
clean:
	rm -f *.o *.elf *.img