# REC -reka compiler-
~~C言語のパクリ~~作者オリジナル言語の"reka"をアセンブリ言語に翻訳します。未完成です。

## Overview
大体コンパイラのセオリーに則っていると思います。
1. 字句解析。入力されたソースコードを分解してトークン列に変換する。
1. 構文解析。トークン列から抽象構文木を作る。
1. 意味解析。右辺値への代入など意味的な不正を検出する。
1. コード生成。アセンブリを出力する。

## Example
<details>
  <summary>長いので折り畳み</summary>
    こんな感じのソースファイルexample.rekaがあるとします。フィボナッチ数列の20項目を計算します。
    <pre>
      <code>
        int fibo(int n)
        {
            if(n==1||n==2){
                return 1;
            }else{
                return fibo(n-1)+fibo(n-2);
            }
        }
        int main()
        {
            fibo(20);
            return 0;
        }
      </code>
    </pre>
    これをこうします。rec.outはソースを適当にコンパイルしてすでに作成済みであるとします。example.rekaを入力として受け取り、アセンブリに翻訳したものをexample.sに出力します。それをexample.outという実行ファイルにするのですが、結局実行ファイルの生成でgccに頼っているため、このソフトウェアの存在意義は謎です。
    <pre>
      <code>
        $ ./rec.out example.reka example.s
        $ gcc -o example.out example.s
      </code>
    </pre>
    現在rekaには入出力の機能が一切ありません。仕方ないのでgdbで正しく計算できてるか確かめます。<br />
    式を評価した値はraxレジスタに格納されるので、fibo(20)をcallした直後のraxを確かめます。<br />
    ついでにRECにはコード最適化機能も未実装です。0を足したり引いたりpushの直後にpopしてたり到達不能コードがあったりしますがご愛嬌です。
    <pre>
      <code>
        $ gdb example.out
        (gdb) disass main
        Dump of assembler code for function main:
           0x0000000000400529 <+0>:     push   %rbp
           0x000000000040052a <+1>:     mov    %rsp,%rbp
           0x000000000040052d <+4>:     sub    $0x0,%rsp
           0x0000000000400531 <+8>:     sub    $0x0,%rsp
           0x0000000000400535 <+12>:    pushq  $0x14
           0x0000000000400537 <+14>:    pop    %rdi
           0x0000000000400538 <+15>:    callq  0x400482 &lt;fibo&gt;
           0x000000000040053d <+20>:    add    $0x0,%rsp
           0x0000000000400541 <+24>:    push   %rax
           0x0000000000400542 <+25>:    pop    %rax
           0x0000000000400543 <+26>:    pushq  $0x0
           0x0000000000400545 <+28>:    pop    %rax
           0x0000000000400546 <+29>:    mov    %rbp,%rsp
           0x0000000000400549 <+32>:    pop    %rbp
           0x000000000040054a <+33>:    retq
           0x000000000040054b <+34>:    add    $0x0,%rsp
           0x000000000040054f <+38>:    mov    %rbp,%rsp
           0x0000000000400552 <+41>:    pop    %rbp
           0x0000000000400553 <+42>:    retq
           0x0000000000400554 <+43>:    nopw   %cs:0x0(%rax,%rax,1)
           0x000000000040055e <+53>:    xchg   %ax,%ax
        End of assembler dump.
        (gdb) start
        Temporary breakpoint 1 at 0x40052d
        Starting program: example.out
        <br />
        Temporary breakpoint 1, 0x000000000040052d in main ()
        (gdb) break *0x40053d
        Breakpoint 2 at 0x40053d
        (gdb) continue
        Continuing.
        <br />
        Breakpoint 2, 0x000000000040053d in main ()
        (gdb) print $rax
        $1 = 6765
      </code>
    </pre>
    計算できてました。
</details>

## rekaの機能
<details>
  <summary>こっちも長いので折り畳み</summary>
  ✅は実装済み、🔵はそのうち実装予定。
  
  ### データ
  ✅64bitローカル整数変数の定義<br />
  🔵定義を伴わない宣言<br />
  🔵signedとunsignedの区別<br />
  🔵グローバル変数<br />
  🔵実数型<br />
  🔵ポインタ<br />
  🔵配列<br />
  
  ### 制御文
  ✅if文、else文<br />
  🔵switch文<br />
  ✅while文<br />
  🔵do-while文<br />
  ✅for文<br />
  🔵break文<br />
  🔵continue文<br />
  🔵goto文<br />
  ✅return文<br />
  
  ### 関数
  ✅定義<br />
  ✅シグネチャのチェック<br />
  ✅プロトタイプ宣言<br />
  
  ### 演算子
  
  #### 算術演算子
  ✅単項プラス<br />
  ✅単項マイナス<br />
  ✅加算<br />
  ✅減算<br />
  ✅乗算<br />
  ✅除算<br />
  ✅剰余<br />
  ✅前置インクリメント<br />
  ✅前置デクリメント<br />
  ✅後置インクリメント<br />
  ✅後置デクリメント<br />
  ✅加算代入<br />
  ✅減算代入<br />
  ✅乗算代入<br />
  ✅除算代入<br />
  ✅剰余代入<br />
  
  #### 比較演算子
  ✅等価<br />
  ✅非等価<br />
  ✅小なり<br />
  ✅大なり<br />
  ✅小なりイコール<br />
  ✅大なりイコール<br />
  
  #### 論理演算子
  ✅論理否定<br />
  ✅論理和<br />
  ✅論理積<br />
  
  #### ビット演算子
  🔵ビット否定<br />
  🔵ビット和<br />
  🔵ビット積<br />
  🔵ビット排他的論理和<br />
  🔵左シフト<br />
  🔵右シフト<br />
  🔵ビット和代入<br />
  🔵ビット積代入<br />
  🔵ビット排他的論理和代入<br />
  🔵左シフト代入<br />
  🔵右シフト代入<br />
  
  #### その他
  ✅単純代入<br />
  ✅関数呼び出し<br />
  ✅カンマ<br />
  🔵条件<br />
  🔵配列添え字<br />
  🔵間接<br />
  🔵アドレス<br />
  🔵直接メンバ<br />
  🔵間接メンバ<br />
  🔵キャスト<br />
  🔵sizeof<br />
  
  ### その他
  ✅変数のスコープ<br />
  ✅ブロックコメント<br />
  🔵行コメント<br />
</details>

## 謝辞
ほぼ[ここ](https://www.sigbus.info/compilerbook)に書いてあることをやってるだけです。マジ感謝。
