= 基礎から説明する暗号通貨

== はじめに

みなさんこんにちは。
このたび、初めて技術記事を執筆することになりました、るなすたです。
今回は、ブロックチェインの基礎技術とその応用について、解説したいと思います。

巷では、暗号通貨の通貨としての、投機対象としての説明はたくさんありますが、
技術的な視点での解説があまり多くないので、これを機に、
ブロックチェインの技術的な面にも興味を持っていただければ幸いです。

この記事では、ブロックチェインの基礎となっている暗号理論から、
暗号通貨の基になった元論文について簡単なまとめと、
その応用技術について、薄く広く取り扱います。

この記事は、Github で公開している「基礎から説明する暗号通貨」@<fn>{github}という
スライドを、記事の形でまとめなおしたものになりますので、
そちらもどうぞよろしくお願いします。

//footnote[github][基礎から説明する暗号通貨: https://github.com/hoshizuki/Cryptocurrency]

== 暗号理論の基礎

暗号理論というと、難しい数式がずらずら並ぶことが多いですが、
暗号通貨を理解する上で、と限定すれば、
難しい数式を使う必要はあまりありません。

まず、暗号理論の中で最も重要な概念である、「一方向性」。
暗号通貨のほとんどの技術はここから始まります。

一方向性とは、データＡとデータＢ、２つあった時に、
ＡからＢへの計算は簡単にできるけれども、ＢからＡを求めるのは
非常に難しい、もしくは事実上不可能である、という性質です。

身近な例で言えば、テストの平均点。
クラス全員の点数から平均点を求めるのはとても簡単ですが、
平均点から各個人のテストの点数を計算するのは不可能です。
このような非対称な性質のことを、一方向性と呼んでいます。

似たような感じで、暗号の世界では「ハッシュ関数」というものがあります。
どんなデータでも、固定長の、乱数のようなビット列に変換する関数で、
その「ハッシュ値」から元のデータを復元することはほぼ不可能と言えます。
MD5 とか SHA-1、SHA-256 など、名前だけは聞いたことがあるのではないでしょうか？

もう１つ、暗号理論の世界で重要なのが「トラップドア付き一方向性関数」というものです。
基本的には一方向性の性質を持っているけれども、ある特別な、秘密の情報を
持っている人にとってだけは、逆算が可能になる、という変わった性質を持つ関数です。

現実世界で例えるならば、南京錠のイメージです。
誰でも南京錠を閉めるのは簡単ですが、開けるのは（壊すのを除けば）難しい、
でも、鍵を持っている人だけは簡単に開けることができる。
そんな性質のことです。

これを逆方向に活用するのが「電子署名」です。
電子署名から元データの検証を行うのは簡単な計算でできますが、
元データから電子署名を作ることは非常に難しい、でも秘密鍵を持っている人だけが
簡単な計算で電子署名を作ることができる。
これが電子署名の基本的な考え方です。

ただ、この考え方だけだと、署名したい人ごとにトラップドア付き一方向性関数を
用意せねばならず、非常に面倒です。そこで、トラップドア付き一方向性関数を
パラメータによってカスタマイズできるように設計しておきます。
このパラメータが「公開鍵」「検証鍵」と呼ばれるもので、
誰でも検証ができるように公開される情報となります。
そして秘密の情報を「秘密鍵」「署名鍵」と呼んで、署名したい人ごとに鍵のペアを
作るようにすることで、トラップドア付き一方向性関数が巷にあふれることを防ぎます。

ブロックチェインを理解するだけであれば、
暗号理論についてはこの程度の知識があれば問題ありません。

== ブロックチェインの基礎理論

ここからは、Bitcoin の元論文@<fn>{bitcoin_paper}、サトシ・ナカモト氏の論文を簡単になぞります。

//footnote[bitcoin_paper][Bitcoin Wiki で公開されています：https://bitcoin.org/bitcoin.pdf]

「新しい持ち主の公開鍵」を「今の持ち主が署名」することで、
所有権の移転とみなします。とりあえず今のところは「何の所有権？」とは明示しません。
持ち主が「確かにこの人に譲渡しましたよ」とサインするイメージです。

この公開鍵と署名の組み合わせを「取引」「トランザクション」と呼びます。
次々と新しい所有者に所有権を移転し続けることで、
トランザクションのチェインが構成できます。

このチェインは、所有権の履歴、歴史を表しています。
そしてチェインの先端が今の持ち主を表す公開鍵となります。
先端の公開鍵に対応する秘密鍵の持ち主は、新たな持ち主に宛てて
署名を作る権利を持っています。この権利こそが「コイン」の実体です。

さて、この時点で既にコインは通貨としてのいろいろな性質を持っています。
例えば勝手に増やすことができないとか、他人に譲渡すると自分のものではなくなるとか、
そういう、いわゆる普通の通貨が持つ性質です。

ただし、コインを支払って何か物を買って、
後から履歴を改ざんすることで支払いをなかったことにする、
という悪用を防ぐことはできません。これを「二重使用」と呼びます。

履歴の改ざんを監視するためにサーバを用意して、
そこのデータベースで流通する全コインの履歴を管理すれば防げますが、
これでは中央銀行が発行し管理している既存の通貨と何ら変わりありません。

そこでサトシ・ナカモト氏の論文では「タイムスタンプサーバ」という物の
採用を提案しています。これは P2P ネットワークでデータベースを持ち、
取引と時刻を紐づけて公開することで、履歴の改ざんをできなくするシステムです。

しかし、P2P ネットワークでデータベースを保守するとなると、
改ざんを目論む悪い人が P2P ネットワークのノードとして混ざる危険があります。

なので、とりあえず「大多数は悪意のないノード」という仮定をします。
この仮定は逆に言えば「悪意のないノードは大多数の計算資源を持つ」となります。
ならばその大多数の計算資源を背景として信頼性を担保するのが、
プルーフオブワーク（PoW）というものになります。

具体的には、適当に難しい問題を全ノードに“早い者勝ちで”解かせます。
早い者勝ちであれば、計算資源で勝つ方の誰かが常に勝ちます。
ここで勝った人は悪意のないノードである可能性が高いので、
データベースに追記する権利を得る、という仕組みです。

その問題には、ハッシュ逆算のように、“答えを見つけるには
総当たりをするしかない”けれども“検証は容易である”という、
一方向性関数のような性質を持つ問題が適切です。

そこで、“ハッシュ値の先頭何ビットかが 0 になる元データを探す問題”という
ものを用意して、それを全ノードに解かせることにします。
元データは「前のブロックのハッシュ値」「トランザクションデータ」「何でもいい数字」
の組み合わせとして、何でもいい部分を変えながら総当たりすることで、
ハッシュ値の先頭を 0 にします。

この仕組みは実際よくできていますが、ただ「信頼性のために計算資源を貸して」と
言っても、ユーザの電気代もタダではないので誰も問題を解いてはくれません。
そこで、ブロックへの追記に成功した場合に報酬を得られることにすれば、
Win-Win の関係になります。まるで鉱脈を掘る鉱夫のようだ、というわけで、
「マイニング」と呼ばれています。

ここで、報酬について考えてみましょう。
これは中央銀行が定期的に新しい通貨を発行し続けているのと同じ状況な訳で、
無限に流通量が増える通貨は間違いなくインフレが発生してしまいます。
そこで、発掘の報酬は 50 コインとして、210,000 ブロックごとに半分にする、
というルールを導入します。計算上、2140 年には報酬が 0 となり、
流通量が制限され、インフレへの懸念はなくなることになります。

ここまでで通貨としての基本的な機能は揃いました。
ただ、取引の単位が１コインだとちょっと大きすぎて不便なこともあるでしょう。
ドルに対するセント、円に対する銭みたいなものを用意しておいた方が良いでしょう。
というわけで、トランザクションの入力と出力には複数のトランザクションを
接続できるようにしておくことを提案しています。

この時、常識的に考えれば「入力の和≧出力の和」の公式が成り立ちます。
差額は「手数料」としてマイニングする人の報酬に含めることにします。

こうすることで、マイニング報酬が 0 になっても、トランザクション手数料を
目当てにマイニングを行う人が継続してくれるという目論みです。
経済的な観点から見れば、マイニング報酬を目指す発掘から、トランザクション手数料を
目当てにした発掘への移行とも見ることができ、より現実の通貨流通に近い性質を
持つようになると言えます。

サトシ・ナカモト氏の論文ではさらに、データベースの容量肥大についても言及し、
解決策を提案しています。どういうことかというと、様々な取引はすべて記録されているため、
いずれ巨大なデータベースとして管理に不都合が生じるという懸念です。
それを解決すべく提案されているのが「マークルツリー」というものです。
トランザクションのハッシュ値同士、２つをつなげて新たなハッシュ値を計算し、
これを再帰的に行うことで１つの「マークルルート」を持つという仕組みです。
あるトランザクションがマークルルートに含まれることを証明するには、
各階層で相手方のハッシュ値を持つだけで証明はできますし、
ネットワーク参加ノード全員が全データを持つ必要がなくなります。

少々長くなりましたが、ここまでがサトシ・ナカモト氏の論文の概要になります。
ここから先は、Bitcoin やそれを取り巻くブロックチェインの話題を
ピックアップして解説していきます。

== Bitcoin の設計と実装

サトシ・ナカモト氏の論文による提案は良くできていますが、
これだけではいろいろと問題があるので、Bitcoin の実装では
いろいろ改善されています。

まずマイニングの難しさの調整。マイニングする人が増えると一瞬で解ける可能性も
ありますし、逆にマイニングする人が減ってしまうと、いつまでたってもブロックが
追加されないという問題があります。そこで、0 とするビットの長さをブロックごとに
変化させるよう実装されています。これが「Difficulty」というもので、
マイニングが速い時は大きく、遅い時には小さくすることでおおよそ10分に1回のペースで
マイニングが進むように調整していきます。

次にブロックチェインの分岐に関する問題。ハッシュ関数と問題の性質上、
当たりは1つとは限りません。また、マイニングする人がたくさんいるため、
チェインが分岐する可能性があります。そこで、新しいブロックが複数報告された場合、
とりあえず一旦どれも正当なものとしておいて、しばらくして長く伸びた方が
正当なチェイン、それ以外は廃棄することで対応します。

=== 51％アタック

元論文では「悪意のないノードが多数」という仮定がありましたが、
もしこれが満たされない場合、どうなるのか？ という考察をします。

まず、マイニング報酬のほとんどを総取りできます。大儲けですね。
さらに、ブロックチェインへ追加するトランザクションを恣意的に選択できます。
嫌いな相手の取引は永遠に追記しない、なんてことも可能になります。
このあたりはまだ通貨として致命的な問題にはなりません。

一番問題となるのが、履歴の改ざんによって二重支払が可能になるという点です。
トランザクションのチェインがあるため、他人のコインを横取りすることはできませんが、
一旦他人に支払ったはずのコインを、全額自分に払い戻す取引に差し替えることは
できてしまいます。たとえ“今現在、他人のものである”コインであっても、
過去に一度でも攻撃者の手元に渡ったことのあるコインであれば、
それ以降の履歴を改ざんすることで、取り戻すことが可能になります。

ではどのような場合に履歴の改ざんが可能になるのか。
それに関してはサトシ・ナカモト氏の論文で考察されています。

仮に履歴改ざんを企む攻撃者が、全計算資源の P％ を得たとします。
正当なノードは反対に 100-P％ の計算資源を持っていることになります。
すると、途中のブロックを改ざんしたとき、改ざん後のチェインの長さと
本来のチェインの長さの差は、P％ の確率で縮まり、100-P％ の確率で広まります。
それを繰り返したときに、一度でも追い越すことができれば、
正当なノードにとっては「長い方を正しい歴史とみなす」ルールに従って
改ざんされた歴史のチェインを伸ばそうとすることになるため、
攻撃者にとっては“追い越すことができるか否か”がポイントとなります。

直感的には“差”が大きいほど追い越すのは大変な感じがします。
なので、ブロックチェインの先頭からトランザクションがどのくらい離れたかを
「確認数」「承認数」などと呼んでいます。

実際にこのモデルは確率過程と呼ばれる分野の「ランダムウォークモデル」という
ものに当てはめて、具体的な攻撃成功確率を計算することができます。
いくつか値を設定してみると、@<table>{attack}のようなシミュレーションができます。

//table[attack][攻撃成功確率のシミュレーション]{
計算量の占有率	確認数	改ざん成功確率
--------------------------------------
51%	144	100%
20%	144	5.27×10^-14%
20%	12	0.029%
20%	10	0.106%
20%	6	1.42%
10%	6	0.024%
5%	6	0.00038%
//}

Bitcoin のオフィシャルな実装では、確認数 6 で一応安全なものとみなす設定が
デフォルトの設定となっています。そして執筆時現在では、
最大手のマイナーが 20％ くらいを占めています。
意外に攻撃成功率は高いと思いませんか？

=== トランザクションスクリプト

トランザクションとは、新しい所有者の公開鍵に、
今の所有者が署名した組み合わせのことでした。

ただ、それだけではあまり複雑な取引が表現できないので、
Bitcoin やその派生コインでは、公開鍵のフィールド、
署名のフィールドの両方を、スクリプト言語で記述するよう実装されています。

この言語は簡単な、いわゆるスタック型仮想マシンの言語となっていて、
公開鍵用のスクリプト scriptPubKey と、署名用のスクリプト scriptSig を連結して
実行したとき、スタックトップが True になっていれば、
正しく署名された取引だとしてトランザクションのチェインをつなぎます。

スタック型仮想マシンについて説明しだすと本題からずれすぎるため、
ここでは「そういうものがあるんだ」という程度の理解で読み進めていただければと思います。

Bitcoin の黎明期は P2PK (Pay-to-PubKey) と呼ばれるスクリプトが使われていましたが、
最近は P2PH (Pay-to-Pubkey Hash) と呼ばれるスクリプトが使われていますので、
そちらをご紹介します。

//list[P2PH][P2PH スクリプト]{
scriptSig
1. 署名をスタックに積む
2. 公開鍵をスタックに積む
scriptPubKey
1. スタックトップ（公開鍵）を複製する
2. スタックトップ（公開鍵）のハッシュを計算する
3. 受け入れる公開鍵のハッシュをスタックに積む
4. スタック先頭の２つの一致を確認
5. スタック先頭の公開鍵と署名を検証する
//}

スタック型マシンの言語を脳内で実行できる方はこれで署名検証ができることがわかると思います。
そうでない人は、ふーん程度に思っておいてもらえば十分です。

== Bitcoin 2.0

=== トランザクションに任意のデータを埋め込む

元々、トランザクションに固有の任意データを埋める領域は存在しませんが、
スクリプトにある命令を利用することで実質的に任意のデータを埋めることはできます。

例えば OP_RETURN という命令。これを実行するとただちに検証に失敗します。
つまり、このトランザクション出力は誰にも消費ができないことを表します。
この命令の後ろには 80Byte (古い実装では 40Byte なので注意) の任意のデータを
書くことができます。

もしくは OP_CHECKMULTISIG という命令。こちらは複数の署名で検証を試みる命令ですが、
署名は 1 つ 32Byte なので、1 つだけ有効な署名を書いておけば残りは好きに使っても
いいのではないか？ という発想です。

=== 任意データ領域に取引情報を書く Counterparty

新しい仮想通貨を作ろうとすると、専用のコインノードを作って参加者を集め、
非常に手間がかかり面倒です。マイナーも集めないといけません。

そんなことをしなくても、既存のブロックチェイン、例えば Bitcoin の
トランザクションの任意領域に取引情報を書けば新しい通貨が作れる！ という発想は
きわめて自然な流れですね。

ただしそれだけではいろいろ問題があります。
例えば流通量のコントロール。Bitcoin ではマイニング報酬を半減させることで
流通量の上限を決めていましたが、このやり方なら無限に流通量を増やすことができます。
例えば最初の通貨分配方法。作った人が上場益的なものを得るのはとても不公平だし、
作った人が知人にこっそり多く分配しているかも知れません。

そこで Counterparty という暗号通貨がとった方法が、プルーフオブバーン。
1CounterpartyXXXXXXXXXXXXXXXUWLpVr という、誰も秘密鍵を知らない Bitcoin アドレスに
送金した額に応じて、Counterparty における通貨、XCP の初期分配額が決まる仕組みです。

これならば流通量は指定期間に送金された分で固定となりますし、
誰がいくら手に入れたかは、Bitcoin の送金履歴を見れば一目瞭然。
誰が見ても公平な初期分配方法となります。

== Atomic Swap

A さんが持つ Bitcoin、B さんが持つ Monacoin、2 人でこれを交換したいことがあります。
いわゆる通貨の売買ですね。

一般的には取引所を介して交換しますが、
やろうと思えばお互いに送金するだけでも一応交換は成立します。

ではなぜ取引所が存在するのか。
それは暗号通貨ではブロックチェインに組み込まれるまでタイムラグがあるからです。
送金のトランザクションは、ブロックチェインに組み込まれるまでは取り消しが効きます。
ということは、後に組み込まれる方の送金トランザクションが取り消されると
「交換」の約束が果たされず、一方的に持ち逃げできてしまいます。

そこで、上手いこと持ち逃げできないようにスクリプトを組んで交換する方法が、実はあります。
それこそが、Atomic Swap です。

原理は簡単で、秘密の値 R をランダムに生成してそのハッシュ値 H を求めます。
scriptPubKey にハッシュ値 H を書いて scriptSig にハッシュ値の元となった R を書きます。
スクリプトとしては以下の感じになります。

//list[AtomicSwapPrin][Atomic Swap の原理となるスクリプト]{
scriptSig
1. 秘密の値 R をスタックに積む
scriptPubKey
1. スタックトップのハッシュ値を計算する
2. 公開されているハッシュ値 H をスタックに積む
3. スタックトップの 2 つの値の一致を確認する
//}

このスクリプトを使って以下の手順で交換を行います。

 1. A さんが秘密の値 R を決めて、そのハッシュ値 H を計算する
 2. scriptPubKey に H を書いて送金する
 3. ブロックチェインに H が公開されるので、B さんはそれを使って scriptPubKey に H を書いて送金する
 4. A さんは最初に決めた R を使って B さんの資金を取り出す
 5. ブロックチェインに R が公開されるので、B さんはそれを使って A さんの送金した資金を取り出す

基本的なアイデアはこんな感じですが、これだけではまだ問題があります。
まず、R が開示されると誰でも A さんが送った資金を取り出せてしまうので、
scriptPubKey に B さんの公開鍵も書き足す必要があります。
また、何らかの理由で A さんが R を開示しなかった場合、お互いに資金を取り出せず、
双方の資金が宙に浮いてしまいます。そこでタイムアップを指定します。

それらの対処をしたバージョンのスクリプトは以下です。

//list[AtomicSwap][Atomic Swap スクリプト]{
scriptSig1 (通常の取引用)
1. 受け取る人の署名をスタックに積む
2. 受け取る人の公開鍵をスタックに積む
3. 秘密の値 R をスタックに積む
4. 1 をスタックに積む
scriptSig2 (巻き戻し用)
1. 送金者の署名をスタックに積む
2. 送金者の公開鍵をスタックに積む
3. 0 をスタックに積む
scriptPubKey
1. IF スタックトップが 1
2.   スタック先頭のハッシュ値を計算
3.   ハッシュ値 H と一致確認
4.   受け取り人の公開鍵と署名を確認
5. ELSE
6.   有効期限を確認
7.   送金者の公開鍵と署名を確認
8. ENDIF
//}

ここでポイントとなるのが、数ある暗号通貨のほとんどは Bitcoin の実装から
派生して開発が進められているという事実です。
つまり、スクリプトもほぼ同じものが使いまわせます。
ということは、Bitcoin 派生のコインはすべて、このスキームが有効と言えます。

== マージドマイニング

いまさらですが、マイニングはすごく無駄の多い仕組みです。
最初に正解を見つけた人以外の計算資源は、途中まで計算をしていたわけで、
それらはすべて無駄な計算をしていたことになってしまいます。

ここで注目するのは、暗号通貨によってその時の Difficulty が異なるという事実です。
その事実に目を付けて活用を試みた暗号通貨に、Namecoin というものがあります。
例えば、Bitcoin は先頭 14bit が 0 になることを要求しているけれども、
Namecoin は先頭 10bit が 0 であれば正解となる、というケースは頻繁に発生します。
ここで Bitcoin のマイニング中に先頭 12bit が 0 になる解を見つけてしまった場合、
Namecoin の回答として流用できれば、少しはマイニング効率が良くなりそうです。

そこで提案されたのが Aux-PoW という仕組みです。
マイニングしていた Bitcoin の前ブロックハッシュ・マークルルートと掘り当てた Nonce を
Namecoin のブロックに組み込み、Namecoin の前ブロックハッシュ・マークルルートと、
ダミーの Nonce でブロックを構築します。

ただこれだけだと、一度掘り当てたマイニング結果を永遠に使いまわせてしまうので、
最初から例えば「Namecoin のブロック 19200 のために Aux-PoW した」という証拠が必要です。

そこで活躍するのが、コインベーストランザクションと呼ばれる特殊なトランザクションです。
これは通常、マイニング報酬として入力が存在せず、報酬分の出力のみを持つ性質のものですが、
入力が存在しないということは、入力の scriptSig は誰も使っていない、
つまりブロックに 1 つ存在する任意領域なわけです。
ここに、先ほどの例えならば「Namecoin のブロック 19200 の前ブロックハッシュ・マークルルートから構築した
ブロックハッシュ」を書き込むルールにします。
こうやって組み立てた特別なコインベースを持つブロックで PoW を行った場合のみ、
Namecoin は Bitcoin の PoW 結果を Aux-PoW として承認します。

Aux-PoW はなかなかに面白い事実を示唆しています。
実は Work を Proof できれば、マイニング対象のデータは自分のブロックチェインの
構成データである必要すらなく、何でもいいんだ、という事実です。
また、Aux-PoW は見方を変えれば Namecoin の値を使った PoW 問題と、
Bitcoin の値を使った PoW 問題の二種類の問題を用意しているだけとも言えます。
このどちらを解いてもよく、判定に使う Difficulty とハッシュ関数が共通である限り、
この 2 つは値が異なるだけで、問題の難しさは変わりません。

適当にネットで調べると、「ハッシュ関数が同じだからマージドマイニングができる」
という説明が散見されますが、仕組みとしてはハッシュ関数の中身については言及して
いないことは明白ですね。本当は問題の難しさの調整の都合上、
“ハッシュ関数が同じ方が都合がいい”というだけの話で、適切に Difficulty に差をつければ
実は異なるハッシュ関数でも理論上はマージドマイニングが可能です。

Aux-PoW によって問題を二種類用意することは、ブロックチェインを新たに作る側にも
メリットはあります。まず親チェインとなる Bitcoin に手を加える必要がないのに
Bitcoin のマイニングユーザを巻き込むことができます。
マイニングする側にとってはオーバーヘッドがほとんどなく両方の通貨の報酬が得られるため、
作ったばかりの無名な暗号通貨でもマイニングユーザが集まりやすくなります。

== スマートコントラクトと Ethereum

ところでブロックチェイン論文の新規性とはなんだったでしょうか？
それは「ビザンチン将軍問題」という難問に対する実用的な解法を提案したところにあります。

ビザンチン将軍問題とは、東ローマ帝国（ビザンチン帝国）を攻略しようとするオスマン帝国の
将軍たちで起きた問題に起源をもつ問題です。
将軍たちはビザンチン帝国を襲撃するか、ここで撤退するかの判断を下す必要があります。
一部の将軍だけで攻め込むのは悪手なので、全員で一致した結論を出す必要があります。
ただし、全員離れた場所に居るため全員集合して多数決を取るわけにはいかず、
将軍のうち何人かは裏切り者であったり、信書も途中で書き換えられてしまったり、
散々な状況です。この状況で全員で一致した結論を出すことは可能なのか？
可能であればどのようにすればいいのか？ という問題です。

ブロックチェイン論文によれば、PoW によって計算試験が多く集まったチェインを
正当なものと合意し、裏切るノードが十分少なく、ブロック作成後、十分に時間が経過すれば
改ざんの成功率は十分に低くなります。この“十分”を現実的なラインで達成できる、
という点で、ブロックチェイン論文は新規性のある論文でした。

ビザンチン将軍問題は P2P ネットワークを構築するときに必ず問題になる点で、
例えば P2P ネットワークで Wiki を作るとしましょう。
中央集権的なサーバを 1 台置けば Wiki は簡単に作れますが、
P2P ネットワークで作るのはビザンチン将軍問題の解決が非常に難しいです。

さて、突然話が変わりますが、暗号通貨の「残高」とはなんだったでしょうか？
次の所有者を自由に決める権利＝コインの所有、でした。
ということは、その権利を足し合わせたもの、つまりすべての未使用トランザクション出力の
合計が「残高」となります。

であれば、ブロックチェインネットワークに存在する NPC 的な誰かに、
Wiki の編集内容を含めて“送金”すると、その NPC の“残高”は、
Wiki の編集内容の集合、つまり“編集履歴”になります。

残高の形になっている編集履歴をタイムスタンプ順に適用すれば、
ブロックチェイン上で動く Wiki が作れます。
NPC は以下の疑似コードを実行してくれれば十分です。

//list[EthWiki][NPC が実行する疑似コード]{
class Wiki {
  string contents;
  void set( string v ) {
    contents = v;
  }
  string get() {
    return contents;
  }
}
//}

この疑似コードを実行できるようにしよう、というのが Ethereum です。

Ethereum では普通のユーザと NPC を区別するため、
アカウントは EOA (External Owned Account) と呼ばれる普通のユーザと、
contract と呼ばれる NPC に分類されます。
contract 宛てに送金することで、疑似コードを実行してコンテンツを更新します。

Ethereum とセットで語られることの多い「スマートコントラクト」という単語。
これは、自動販売機のように、取引の一方を自動で代行してくれるシステムのことを指します。
Ethereum はまさに代金を受け取ることで Wiki の更新を行ってくれるように、
取引の代行をしてくれる bot を作るフレームワークとして機能します。
自動販売機開発プラットフォームとでも言えばいいでしょうか。
大体そんなイメージで理解してもらえば間違いありません。

== おわりに

以上、ざっくりとですが、暗号通貨の基本的な部分と、様々な応用技術について簡単に触れました。
暗号通貨はただの投機の対象ではなく、技術的にもなかなかに面白いものであるというところが
伝われば幸いです。それではまたどこかで機会がありましたら。
