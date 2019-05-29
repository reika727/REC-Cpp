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
        int print_num(int num);
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
            print_num(fibo(20));
            return 0;
        }
      </code>
    </pre>
    この1行目にあるのはprint_numのプロトタイプ宣言です。rekaには入出力の機能が未実装なので、そこはC言語で書いて後でリンクします。<br />
    このファイルをfunc.cとします。
    <pre>
      <code>
        #include&lt;stdio.h&gt;
        int print_num(int num)
        {
            printf("%d\n",num);
            return 0;
        }
      </code>
    </pre>
    これをこうします(rec.outはソースを適当にコンパイルしてすでに作成済みであるとします)。<br />
    example.rekaを入力として受け取り、アセンブリに翻訳したものをexample.sに出力します。<br />
    それをexample.outという実行ファイルにするのですが、結局実行ファイルの生成でgccに頼っているため、このソフトウェアの存在意義は謎です。
    <pre>
      <code>
        $ ./rec.out example.reka example.s
        $ gcc -o example.out example.s func.c
        $ ./example.out
        6765
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
