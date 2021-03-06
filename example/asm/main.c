/*
	インラインアセンブリコードを利用し、コンソールに文字列を表示します。

	[解説]
		インラインアセンブリから二通りの方法でコンソールに文字列を表示します。

		一つ目の方法では、C 標準関数を利用します。
		インラインアセンブリから C 関数を実行するには、引数を push したのちに
		関数名に _ を付けたラベルを jsr で呼び出す必要があります。

		二つ目の方法では、X68K の doscall を利用します。
		doscall は、0xFF から始まる 16 ビットの未定義命令をユーザープログラムに
		実行させることで、OS が提供しているファンクション・コールを呼出します。

		doscall を利用するには、ファンクション番号を定義した、doscall.mac と
		呼ばれるファイル（SHARP Compiler PRO-68K ver2.1 に収録されている）を、
		アセンブラソースコードの冒頭で include する必要があります。
		x68k_gas2has.pl に引数 -inc を指定することで、この include を行う
		ディレクティブを生成することができます。
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	/* C 標準関数の puts を使ったコンソール文字列出力 */
	{
		static char s_string[] = "hello world. (by puts())";
		asm volatile (
			"	move.l	%0,-(sp)\n"
			"	jsr		_puts\n"			/* 外部シンボルを参照する時は _ を付ける */
			"	addq.l	#4,sp\n"
		:	/* 出力 */
		:	/* 入力 */	"irm" (&s_string)	/* 引数 %0 */
		:	/* 破壊 */	"d0"				/* C 関数は d0 に結果を返すか破壊する */
		);
	}

	/* doscall を使ったコンソール文字列出力 */
	{
		static char s_string[] = "hello world. (by doscall)\r\n";
		asm volatile (
			"	move.l	%0,-(sp)\n"
			"	dc.w	_PRINT\n"			/* doscall.mac で "_PRINT equ 0xff09" が定義されている */
			"	addq.l	#4,sp\n"
		:	/* 出力 */
		:	/* 入力 */	"irm" (&s_string)	/* 引数 %0 */
		:	/* 破壊 */	"d0"				/* doscall は d0 に結果を返すか破壊する */
		);
	}

	return 0;
}
