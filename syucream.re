= 今日から始める IOTA

はじめまして、あるいはお久しぶりです。普段はとある Web サービスを支えるお仕事をしている @syucream です。
この記事では暗号通貨のひとつである IOTA (アイオータ)について紹介する内容になります。
暗号通貨の基礎の話はあまり説明しません。それについてはこの本の「基礎から説明する暗号通貨（仮）」をご参照ください。


== IOTA とは

IOTA @<fn>{iota_official} は暗号通貨の一種であり、 IoT(Internet of Things) を支える基盤の一つを目指しています。
ちなみにこの記事を執筆している 2018 年 3 月現在では、日本国内の主要な取引所では残念ながら IOTA の扱いはありません。
そのため、もし IOTA を手早く手に入れたい場合は、国外の取引所を利用する必要があるでしょう。

IOTA はブロックチェインではなく Tangle という DAG(Directed Acyclic Graph, 非循環有向グラフ) の仕組みによってトランザクションを管理します。
暗号通貨と言われてすぐに思いつくのは BitCoin や Ethereum かと思いますが、 IOTA は主にこれらと比べて以下のような違いがあります。

 * トランザクション手数料がかからない
 * MAM(Masked Authenticated Message) という閲覧者を絞ったメッセージを公開できる
 * スケーラブルで非中央集権なトランザクション管理
 * 細かなトランザクション向き

以下ではこれらの特徴について詳しく見ていきます。

//footnote[iota_official][IOTA 公式: https://iota.org/]

=== トランザクション手数料がかからない

これは IOTA の大きな特徴と言えるでしょう。
BitCoin や Ethereum ではトランザクションを承認するマイニングをする人にに支払う手数料が発生しますが、 IOTA ではこれがありません。
BitCoin などで送金しようとした際に、場合によってはこの手数料が送信金額を上回る可能性も考えられますし、あまり気軽に送金処理が行えなくなる場合が考えられます。

トランザクション手数料が無料であることは、 IOTA の仕組みで送金以外の意図をもたらすのにも使えるかも知れません。
例えば 1i(IOTA の最小貨幣単位) を送る毎に何らかのカウンタをインクリメントしたことに見立てるなど。
BitCoin など他通貨でも同じことは可能ですが、手数料の有無は運用し続ける上でやはり気になってくる点だと思われます。


=== MAM(Masked Authenticated Message) という閲覧者を絞ったメッセージを公開できる

MAM は IOTA のトランザクションのメッセージを暗号化し、閲覧者を絞ってそのメッセージを複合可能にする仕組みです。
暗号通貨のトランザクションというと公開されているイメージが思いますが、 IOTA ではこの仕組みによって機密性が高い情報もやり取りすることが可能です。

MAM ではメッセージを読み出し可能な閲覧者を 3 種類のポリシーに分けて送信することが可能です。
イメージとしては下図のように、誰でも複合可能な public 、 key を共有された人のみが複合可能な restricted 、メッセージの root の値を持つ、送信した人のみが知りうる情報を持つ人のみが複合できる private があります。。

//image[syucream_mam][MAM 閲覧可能範囲のイメージ]

MAM についての解説は日本コミュニティの解説記事 @<fn>{mam} が丁寧に記述されており、取っ付きやすいものかと思われます。

//footnote[mam][MAM: https://iotafan.jp/developer/deep_drive_into_mam_20171229/]

=== スケーラブルで非中央集権なトランザクション管理

ブロックチェインと比較しての話ですが、 DAG によって細かくトランザクションが管理されることでより並列にトランザクションの検証ができるようになるため、スケーラブルであると言われています。
またブロックチェインベースの暗号通貨によくあるマイニングをする人を必要としないのでより非中央集権的なネットワークであるといえます。

さらに iotatoken 社の共同創業者である Dominik 氏による IOTA 紹介プレゼン資料 @<fn>{iota_intro} ではオフラインでもトランザクションを発行でき、オンラインになった際に本流である後述の Tangle に合流することも想定されています。
これにより Tangle に対してもネットワーク的に依存性が低く、 IoT デバイスなどネットワーク環境がある程度制限される場合にも利用可能になると考えられます。

//footnote[iota_intro][A Primer on IOTA: https://blog.iota.org/a-primer-on-iota-with-presentation-e0a6eb2cc621]

=== 細かなトランザクション向き

IOTA は IoT センサーデバイスからのデータ送信の為に使われることも想定しています。
また前述の通りトランザクション手数料がありません。
そのためセンサーデータを細かく送出していくようなマイクロトランザクションに向いているといえます。

マイクロトランザクションについては今後面白い活用方法が出て来る可能性があります。
例えばタクシーの料金支払いを走った距離ごとにマイクロペイメントを実行していき、降りる際は何も手続きをする必要が無い、ような未来も実現できるかも知れません。


== Tangle について

IOTA の紹介の際に出てきた Tangle は、 IOTA を特徴付ける仕組みのひとつとも言えます。
Tangle の white paper は公開されており、また有志により日本語訳も存在します。 @<fn>{tangle_wp} 

Tangle ではブロックチェインのような、幾つかのトランザクションをブロック単位にまとめて繋いで行くのではなく、トランザクション間が細かく連結した有向グラフを描くような形で管理されます。
イメージとしては下図のように、トランザクション間の承認の関係がループの無い有向グラフで構成されるイメージになります。

//image[syucream_tangle][DAG によるトランザクション管理][scale=0.7]

IOTA の Tangle では、新しいトランザクションを送出したい人が他の 2 件以上のトランザクションを承認する必要があります。
前に触れた手数料無料の理由はこれで、マイニングに相当する仕事を自分でやることで手数料の発生を除去しているのです。

なお、他のトランザクションを承認する上で BitCoin などと自分でプルーフオブワークすることになります。
この処理はそれなりにマシンパワーを必要とするのでそれを念頭に置く必要があるのと、 IoT デバイスで送金する場合にはタイミングの調整などを気にする必要が出て来るかも知れません。

//footnote[tangle_wp][Tangle white paper: https://iotafan.jp/wp/iota-wp-jp/]


== IOTA を触ってみる

解説だけではいまいちピンとこない部分が多々あると思われます。
ここでは実際に IOTA に触ってみましょう！

=== 事前準備

IOTA を触ってみるには実際に IOTA の Wallet を使って送金などの操作をしてみるのが簡単でしょう。
しかし送金するにしてもまずは肝心の IOTA が手元に無ければなりません。

IOTA の最も簡単な入手方法は、すでに IOTA を保有する誰かから送金してもらうことです。
最小の 1i であればそれほど価値は高くないのですが、都合よく送金してくれる人が見つかるとも限りません。
そんな訳で今回は事前準備として IOTA の購入方法を紹介します。

日本円での入金から IOTA の入手までの概要は下図のようになります。
今回筆者は、国内では Bitflyer 、国外では Binance といった暗号通貨取引所を利用しました。
Binance では現在、日本円で直接取り引きを行なうことができず IOTA は BTC(BitCoin) もしくは ETH(Ethereum) 建てで購入する必要があります。
今回はまず国内の Bitflyer で日本円から BTC を購入し、その後 BTC で IOTA を購入するという手順を踏みました。

//image[syucream_iota_purchase][IOTA 購入フロー]

これでやっと IOTA が手元に確保できました！

=== IOTA Wallet を使ってみる

それでは次に、 IOTA Wallet を用いて IOTA 上で送金を行ってみましょう。
ちなみに IOTA Wallet は GitHub でオープンソースソフトウェアとして公開されており、  @<fn>{iota_wallet} Release から インストーラを取得できます。

//footnote[iota_wallet][IOTA Wallet: https://github.com/iotaledger/wallet/releases]

IOTA Wallet を用いる上で、自前のアカウントを生成しておくと良いでしょう。
IOTA のシード値は特殊なトライトを用いることになります。以下のワンライナーでランダムな値を生成しておくと良いです。

//cmd{
# Linux を想定
$ cat /dev/urandom |tr -dc A-Z9|head -c${1:-81}
//}

IOTA Wallet にログインしたら、予め購入した IOTA を上記で生成したシード値に対応した送金用アドレスに対して送金しておきましょう。
以下のように受取画面から送金用アドレスを取得し、暗号通貨交換所の送金先にそれを設定して送金します。
（送金用アドレスは使い捨てだと考えましょう。使い回すのはリスキーです）

//image[syucream_iota_accept][IOTA 受取用アドレス生成画面][scale=0.7]

実際に IOTA Wallet を用いて送金する場合は以下のように受取用のアドレスと送金額を入力して行います。
送金に際して若干の時間を要します。

//image[syucream_iota_transfer][IOTA 送金画面][scale=0.7]

これだけで送金処理が行えます！
前述の通り IOTA であればトランザクション手数料がかからないので、最小の送金単位である 1i 送金したとしてもそれがそのまま受取口座に届きます。

なお IOTA の仕組みが発展途上なせいか、送金処理はペンディング状態から進まなくなる場合がしばしばあります。
そういった場合には取引履歴から送金トランザクションのリアタッチを行ってみましょう。

=== スクリプトから 送金やメッセージ送信をしてみる

IOTA Wallet を使っただけでは技術的な面白味が感じられないでしょう。
次は IOTA を使って送金やメッセージ送信をするスクリプトを書いてみましょう。

IOTA は公式では JavaScript, Python, Java ライブラリを提供しています。 @<fn>{iota_libraries} 
特に JavaScript 向けライブラリである `iota.lib.js`  @<fn>{iota.lib.js} 開発が進んでいるようです。
というわけで以下では JavaScript ライブラリを使って送金などの処理を実装してみます。

//footnote[iota_libraries][IOTA Client Library: https://iota.readme.io/docs/overview]

`iota.lib.js` は npm などでインストール可能です。

//cmd{
$ npm install iota.lib.js
//}

1i の送金を行なう JavaScript のコードは例えば以下のように記述できます。
ここで 送金時に付与するメッセージとして、 `sendTransfer()` に渡すパラメータに `message` というトライトエンコードされた文字列を埋め込んでみます。
ちなみに執筆現在では IOTA Wallet で送金する分には最低でも 1i 以上の金額を送らなければバリデーションで弾かれてしまうのですが、このスクリプトで送金する文には 0i でも実行できます。

なお、筆者の試した限りだと provider として入力するフルノードは、場合によっては挙動が安定しない場合があります。
どうしてもうまくいかない場合は幾つかのフルノードを試してみましょう。


//listnum[transfer.js][送金コード][js]{
let IOTA = require('iota.lib.js');

const receiver_seeds = '<受取側の seed 値>';
const sender_seeds = '<送金側の seed 値>';

let iota = new IOTA({
    'provider': 'http://<フルノードの host/port>'
});

iota.api.getNewAddress(receiver_seeds, (err, dest_addr) => {
    if (err) {
      console.error(err);
      return;
    }
    iota.api.sendTransfer(sender_seeds, 2, 14, [
        {
          'address': dest_addr,
          'value': 1,
          'message': iota.utils.toTrytes('test'),
          'tag': iota.utils.toTrytes('test'),
        }
      ], (err, data) => {
          if (err) {
            console.error(err);
            return;
          }
          console.log('transfer done.');
    });
});
//}

ここで送信したトランザクションの情報を手軽に確認するには、 TheTangle という取引情報を閲覧できるサービスを用いるのが良いです。 @<fn>{the_tangle} 
以下のように送金先アドレスで検索を行うことで、上記で設定したメッセージやタグが送信出来ていることが確認できます。
ちなみにここでタグの値として設定されている "HDTCGDHD9999999999999999999" は末尾が 9 fill された、 "test" がトライトエンコードされた値です。

//image[syucream_iota_transaction][トランザクションの内容確認画面][scale=0.8]

次に、前述の MAM を送出するスクリプトを実行してみましょう。
MAM の送信ロジックは JavaScript 向けでは iota.lib.js とは別のモジュールである mam.client.js  @<fn>{mam.client.js} にまとめられています。
MAM の IOTA 公式実装は Rust で書かれていて、 @<fn>{MAM} mam.client.js はそれの JavaScript バインディングを提供している形になります。

さて mam.client.js ですが、こちらは npm でコマンド一発でいい感じに導入できるようには現状なっていないようです。
README の記述に従って環境を整えてみましょう。

//cmd{
$ git clone --depth 1 https://github.com/iotaledger/mam.client.js.git
$ cd mam.client.js/
$ yarn
$ git submodule update --init --recursive
//}

実際に mam.client.js を使って MAM でメッセージを送信するコードの例を以下に示します。
このコードでは任意のテキストを MAM の送信ポリシーである public, restricted, private のいずれかに基づいて送信することができます。
今回は送信後の MAM をすぐに fetch して読み出しています。

面白いのは restricted モードでの振る舞いでしょうか。
ここでは secret_tryte 変数が示すシークレットがあれば fetch する側がメッセージを読める様になっています。
メッセージを読み出したい人たちにのみ secret_tryte を渡しておくことで、その人達向け限定のメッセージ送信が可能になります。
これを用いることで例えば貴重なデータを restricted モードで送信しまくっておいて、それを読みたい人が secret_tryte を購入して読み出すなんて購読モデルが構築できるかも知れません。

//listnum[mam.js][MAM 送信コード][js]{
let IOTA = require('iota.lib.js');
let Mam = require('./lib/mam.node.js');

const secret_tryte = 'EVERYDAYFRIDAY';

if (process.argv.length != 4) {
  console.log('Usage: node mam.js <\'message\'> <public|restricted|private>');
  process.exit();
}
let msg = process.argv[2];
let mode = process.argv[3];

if (['public', 'restricted', 'private'].indexOf(mode) == -1) {
  console.log('specify an appropriate mode');
  process.exit();
}

let iota = new IOTA({
    'provider': 'http://<フルノードの host/port>'
});
let mamState = Mam.init(iota);
mamState = Mam.changeMode(
  mamState,
  mode,
  (mode == 'restricted') ? secret_tryte : null
);

const publish_fetch = async packet => {
  let message = Mam.create(mamState, packet);
  console.log('Message Root: ', message.root);
  console.log('Message Address: ', message.address);
  mamState = message.state;
  await Mam.attach(message.payload, message.address);

  await Mam.fetch(
    message.root,
    mode,
    (mode == 'restricted') ? secret_tryte : null,
    console.log
  );
};

publish_fetch(msg);
//}

//footnote[the_tangle][TheTangle.org: https://thetangle.org/]
//footnote[iota.lib.js][iota.lib.js: https://github.com/iotaledger/iota.lib.js]
//footnote[mam.client.js][mam.client.js: https://github.com/iotaledger/mam.client.js]
//footnote[MAM][MAM: https://github.com/iotaledger/MAM]


== おわりに

IOTA の紹介記事、いかがでしたでしょうか？
個人的には手数料無料であるとか MAM といった他の暗号通貨には無い特色を持つ IOTA の活躍に胸を踊らせています。
現段階では発展途上の通貨である感覚が否めませんが、今後単なる通貨のイメージの枠を超えた発展を遂げる可能性があるのではないかと期待しています。

さて（色々な意味で）盛り上がっている暗号通貨界隈、 IOTA は他の通貨とは一風変わった戦略で売り込んでおり個人的には興味深いものであると感じています。
IOTA 共々ウォッチしていきたいところですね！
