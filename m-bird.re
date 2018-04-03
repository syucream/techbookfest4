= エンジニアのためのDNS（再）入門（初級者編）
#@# タイトルどうするか？

== はじめに

「インフラ屋にはなるな。とくに、メールとDNSには手を出すな」。
そう言い遺し、社会という名の地獄巡りへと旅立っていた諸先輩方。
そんな遺言を守ることなく、道を踏み外してしまったインフラ系@<fn>{infra-engineer}エンジニア、えむばーど(@m_bird)です。

//footnote[infra-engineer][飲酒してフラフラのエンジニアの意]

そんな「無くてはならないけれど、なんだか面倒」というイメージの強いDNS。「知っている」つもりになってはいるけれど、意外と理解できていない……そんな声がときどき聞かれます。

本誌の記事は、そんな方々へ向けたものです。
DNS関連の話題に付いていくための基礎知識を得ることを目標とし、DNSの入門からお話を始めます。そして、続く@kdmnの特集にて、DNS関連の話題へ付いていくための必須の知識「ゾーンと委任」について、掘り下げます。

== DNSって、なに？

//image[m-bird_dns01][DNSってなんだろう？][scale=0.6]

DNS、ドメインネームシステム。たった一つの「ドメイン名空間」を成し、この名前空間を利用してドメイン名から情報を取得する「名前解決」をおこなうことができる仕組みです。
そして、私たちが利用したり、管理したりする「DNSサーバー」は、このシステムを構築したり利用したりする要素のひとつです。

まずは、この「ドメイン名システム」の全体概要図を示した図を眺めてみましょう。
ドメイン名システムは、ドメイン名を管理するための「ドメイン名空間」と、そのドメイン名空間を構成する「権威DNSサーバー」、そしてドメイン名の名前解決を行う「フルリゾルバー」などの要素から成り立っています。

//image[m-bird_dns-all][ドメイン名システム(DNS)全体概要]
#@# TODO 名前解決、という言葉が幾度か出てくるが、もう少し厳密に定義する？

@<img>{m-bird_dns-all} を参考にしながら、DNSを用いた名前解決の大まかな流れを見てみましょう。

 1. WebブラウザはWebサーバーにアクセスするために、ドメイン名からIPアドレスを検索する「名前解決」を行います。
    これは、OSが提供する機能@<fn>{getaddrinfo}を利用しておこなわれます。
 2. OSは「フルリゾルバー」に問い合わせを行います。
    @<br>{}ただし、hostsファイルに該当するドメイン名がある場合、hostsファイルの中身を返します。DNSサーバーには問い合わせを行いません。
 3. フルリゾルバーは、「権威DNSサーバー」に問い合わせを行い、名前解決を実施します。
 4. 以降、逆順を辿って結果がOSに返されます。

この流れを頭の片隅に置きつつ、それぞれの要素を見ていきましょう。

//footnote[getaddrinfo][具体的には、@<code>{getaddrinfo()}など。libcが提供している。]

===[column] 「DNSはIPアドレスを引くもの」？

ドメイン名とDNSについて尋ねてみると、「名前とIPアドレスを結びつけるもの」という回答がよく返ってきます。人が覚えづらいIPアドレスを、人が覚えやすいドメイン名に対応づけるのだ、と。

だいたいあっています。よく利用される用途としては、IPアドレスとドメイン名の結びつけに利用されることが多いです。しかし、他の用途もあります。具体的なものとしては、ドメイン名に任意のテキスト情報をドメイン名と結びつけることのできる、「TXTレコード」の存在があります。

===[/column]

==== 階層を「利用する」DNSサーバー
まずは階層を利用するDNSサーバーについて見ていきましょう。
その名も「フルリゾルバー」。人によっては、「キャッシュDNSサーバー」と呼ぶ人もいます。
このDNSサーバーの役割は、@<strong>{ドメイン名空間の階層構造を利用して名前解決を実施する}ことです。

//image[m-bird_full-resolver][フルリゾルバー]

皆さんがお手元で直接利用するPCで、「DNSの設定をする」際に設定しているのはこのサーバーのことを指しています。
Windowsであれば「優先DNSサーバー」「代替DNSサーバー」、Macだと「DNSサーバー」という設定項目となっています。
FreeBSDやLinuxといったUnix系OSでは、@<code>{/etc/resolv.conf}に設定している内容です。
Google の提供するPublic DNSサーバーを利用し、@<code>{8.8.8.8}や@<code>{8.8.4.4}を設定している人もいるかもしれません。

また、フルリゾルバーは名前解決した結果をキャッシュする機能もあります。
これにより、実行コストの高い「ドメイン名の階層構造を利用した名前解決」を何度も実行することを回避しています。人気なドメインであればキャッシュに乗りやすく、応答を高速に返すことが可能になります。
このような特性より、フルリゾルバーは多くのユーザが共用して利用すると、高速に結果を得ることができるため、ISPや会社組織単位などで運用が行われている場合が多いです。

//image[m-bird_full-resolver_cache_exists][フルリゾルバーのキャッシュ][scale=0.8]

では、フルリゾルバーはどのように処理を行い、フルリゾルバーのユーザに応答を返しているのでしょうか。
@<code>{www.example.jp}を例に、フルリゾルバーがドメイン名からホストのIPアドレスを取得する例をみてみましょう。

===== キャッシュが「ない」場合

 1. フルリゾルバーは、まずルートサーバーに対して、問い合わせを行います。
 2. ルートサーバーは、「@<code>{.jp}の権威DNSサーバーに聞いてよ」と返します。
 3. フルリゾルバーは、@<code>{.jp}の権威DNSサーバーに問い合わせをします。
 4. @<code>{.jp}の権威DNSサーバーは、「@<code>{example.jp}の権威DNSサーバーに聞いてよ」と返します。
 5. @<code>{example.jp}の権威DNSサーバーは、「知ってるで、@<code>{www.example.jp}は@<code>{203.0.113.63}やで」と返します。

「え？フルリゾルバーはどうやってルートサーバーを知るの？」という疑問を持たれた方。@<strong>{鋭い}。
フルリゾルバーは、@<strong>{ルートヒント}@<fn>{roothint}と呼ばれる情報をあらかじめ持っているため、この情報に基づいて問い合わせを行います。

//footnote[roothint][ルートヒントについては、中級編にてもう少し踏み込んだお話をします]

===== キャッシュが「ある」場合

 * TODO
 ** 図が要

==== 階層を「成す」DNSサーバー
ドメイン名空間の階層を利用するDNSサーバーがあれば、階層を構成するDNSサーバーがあります。
「権威DNSサーバー」と呼ばれるサーバーが、その役割を担います。
こちらも別名で呼ぶ人がおり、「コンテンツサーバー」と表現されていることがあります。

権威DNSサーバーは、ドメイン名空間の一部を管理します。
例えば、ルートサーバーはドメイン名空間すべての管理権限を持っています。
このうち、.netのドメイン名については.netの権威DNSサーバー、.jpのドメイン名については.jpの権威DNSサーバー……といったように、その管理権限を「委任」して管理しています。

ドメイン名空間が切り出し、その管理権限を別の権威DNSサーバーに委任したいときは、次のステップを踏みます。

 1. サブドメインを作成する
 2. サブドメインの管理権限を委任する

この結果、切り出されて管理権限を委任された名前空間のことを「ゾーン」と呼びます。
また、委任する側が「親」と呼ばれ、委任される側が「子」と呼ばれます。

権威DNSサーバーは、フルリゾルバーからのリクエストを受けて、自分の管理するゾーンの情報を返します。
このとき、管理権限を別の権威DNSサーバーに委任していた場合には、「このDNSサーバーに聞いて」という応答を返します@<fn>{aabit}。

//footnote[aabit][DNSの応答に「@<code>{aa}フラグ」というものがある。通常問い合わせたゾーンの権威DNSサーバーからの応答はオンであるが、他の権威DNSサーバーに委任している場合はオフとなっている。]

#@# memo
#@# * 権威DNSサーバー、DNSコンテンツサーバー
#@# * 「権威ある応答」については触れない
#@# * 自分の持つゾーンの問い合わせについて、他のサーバーの助けを借りることなく応答する
#@# ** ローカルに持っているゾーンの内容に基づいて応答できる
#@# ** 問い合わせに関する処理の過程で、他のDNSサーバーに問い合わせを送る必要はない
#@# ** 本稿では触れないが、自分の持つ情報を更新する仕組みもある（ゾーン転送・notify・Dymanic Update）
#@# ** 自分の持つゾーンについては応答のDNSメッセージでAA bit（auhtoritative bit）をonにする: 名前の由来
#@#    （触れない）

===[column] DNSサーバーに優先度はない？

良く尋ねられるものとして、「DNSサーバーの優先度の設定方法」がありますが、 そんなものは、ありません。

まずは、権威DNSサーバーについて。権威DNSサーバーの優先度はどれも一律で、問い合わせの優先度付けを制御することはできません。
ドメイン名に複数台の権威DNSサーバーの設定をした場合、どの権威DNSもまんべんなく問い合わせが来るかたちになります。

誤解を招く理由の一つは、プライマリサーバー（マスターサーバー）や、セカンダリサーバー（スレーブサーバー）という言葉が、権威DNSサーバーの用語の中に存在することです。
これは、権威DNSサーバーを管理をする際の用語です。複数台の権威DNSサーバーを運用し、権威DNSサーバーのゾーン情報を更新する場合に、複数台の権威DNSサーバーのゾーン情報を個別に直接変更するのは骨が折れます。

このため、1台の権威DNSサーバーの情報を変更し、残りの権威DNSサーバーにその設定を転送する（ゾーン転送）という技術が存在します。このときに、転送元のサーバーをプライマリサーバー、転送先のサーバーをセカンダリサーバーと呼びます。

なので、「プライマリサーバーは最初に問い合わせる先、セカンダリサーバーはプライマリサーバーが応答しない場合に次に問い合わせる先」という意味ではありませんし、そういう制御は不可能です。

フルリゾルバーについても同様に、クライアントからの問い合わせ優先度を制御することはできません。
DNSの利用側であるOS側で複数フルリゾルバーの設定を行い、その優先度を付けることは可能です。しかし、それはDNSの機能ではなく、DNSを利用するクライアント側の機能です。

===[/column]

== 「ドメイン名」とは
さて、名前解決の仕組みを理解したところで、ドメイン名について改めて振り返ってみましょう。
ドメイン名はドメイン名空間のツリー構造を表現するための工夫が凝らされ、ドメイン名空間の階層構造と一致するかたちとなっています。

ドメイン名は、ドットと文字列から構成されており、ドットで区切らた部分を「ラベル」と呼びます。
このラベルの切れ目では、他のサーバーに管理権限を委任することができるようになっています。

ドメイン名は、右から順にトップレベルドメイン、セカンドレベルドメイン、サードレベルドメイン……と呼ばれ、
右に行くほど親側（委任する側）、左に行くほど子側（委任される側）となります。

//image[m-bird_domain-name][ドメイン名の構造][scale=0.4]

例として、@<code>{www.example.co.jp}をみてみましょう。
この場合、@<code>{jp}がトップレベルドメイン名、@<code>{example}がセカンドレベルドメイン名、
@<code>{www}がサードレベルドメイン名です。
また、@<img>{m-bird_dns-all}をみると、@<code>{jp}と@<code>{example}の区切りで委任されていることが分かります。

#@# memo
#@# * ツリー構造
#@# * 階層構造と一致するような形になっている
#@# * 例: www.example.com
#@# * 「.」で区切られたラベルで構成される
#@# * ラベルの切れ目で、他のサーバーに委任することができる
#@# ** 右に行くほど親側（委任する側）、左に行くほど子側（委任される側）
#@# ** 委任すると、そこから先は委任先で自由に管理できる

=== ドメイン名空間のかたち

 * TODO
 ** ゾーンはドメイン名空間の一部
 ** リソースレコードのまとまり
 ** 権威DNSサーバーはそのゾーンを持っている
 *** 空間はゾーンに千切れている

=== Webブラウザが名前解決するまでの流れ
最後に復習として出す。


== まとめ

以上、DNSの全体を眺め、主要な用語や構成要素について学んできました。

この知識を用いて、引きつづき中級者編をお楽しみください。
