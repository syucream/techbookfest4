= エンジニアのためのDNS（再）入門（中級者編）（仮）

== はじめに

前章では初級者編として、DNSを構成する要素（ドメイン名空間・権威DNSサーバー・フルリゾルバー）について紹介しました。
本章では、世の中の権威DNSサーバーたちが作り上げているドメイン名空間について、ゾーンと委任という切り口でもう少し詳しく説明します。

本章で用いている用語は可能な限り、英語についてはRFCにて用いられているものを、日本語についてはRFCに近いか一般的に用いられているものを採用しています。

== ゾーン

#@# 説明がぬるい

ドメイン名空間は、各ノードにラベルを持つツリー構造になっています。

 * ドメイン名空間の一部
 ** サブツリーを構成するレコードのまとまり

@<kw>{リソースレコード, resource records}（以下レコード）

== ルートゾーン

@<kw>{ルートゾーン, root zone}とは、ドメイン名空間を構成するツリーの根（root）となるゾーンです。
ドメイン名空間は、ルートゾーンを根として持ち、その子孫であるたくさんのゾーンから構成されています。

全てのフルリゾルバーは、ルートゾーンとそれを持っている権威DNSサーバーについての情報を持っています。
具体的には、@<kw>{ルートヒント, root hints}と呼ばれる下記の情報を持っています。これらは、ルートゾーン自身にも含まれています。

 * ルートゾーンのNSレコード
 ** つまり、ルートDNSサーバーのホスト名
 * 上記ホストの@<tt>{A}レコードおよび@<tt>{AAAA}レコード
 ** つまり、ルートDNSサーバーのIPv4・IPv6アドレス

フルリゾルバーは、このルートヒントの情報とともに配布されています。ルートヒントに記載されているサーバーを起点に、ドメイン名空間のツリーをルートからたどって、名前解決を行っています。
最新のルートヒントの情報は、@<tt>{https://www.iana.org/domains/root/files}にて公開されています。

なお、ルートヒントの情報を直接使うのではなく、最新の情報を使うために、ルートゾーンに含まれるルートヒント情報を取得し直す実装もあります。この取り直す動作のことを、@<kw>{プライミング, priming}と呼びます。

ルートゾーンの管理に関するポリシーの策定は@<kw>{ICANN, Internet Corporation for Assigned Names and Numbers}によって行われています。実際の運用は@<kw>{IANA機能運用者, IANA Functions Operator}として選ばれた@<kw>{PTI, Public Technical Identifiers}が行っています。

ルートゾーンをサービスする権威DNSサーバー（本稿執筆時点では@<tt>{[a-m].root-servers.net}）のことを、@<kw>{ルートサーバー, root servers}または@<kw>{ルートDNSサーバー}と呼びます。ルートDNSサーバーは、IANAが管理する真正なルートゾーンをインターネットに提供する役割を持っており、複数のオペレーターによって運用されています。詳しくは、@<tt>{http://www.root-servers.org/}を参照して下さい。

===[column] ツリーの「ルート」はただ1つ

たまに、@<tt>{com}ゾーンを持つ権威DNSサーバー（本稿執筆時点では@<tt>{[a-m].gtld-servers.net}）のことを「@<tt>{com}のルート」と呼ぶ人もいます。しかし前述の通り、DNS名前空間は1つのツリーからなっているので、「ルート」とはその頂点を指すことになります。このため、それ以外の部分をルートと呼ぶことはふさわしくありません。

===[/column]


== 委任

前節で、ドメイン名空間はルートゾーンを頂点として、その子孫であるたくさんのゾーンから構成されていると説明しました。
この節では、たくさんのゾーンを1つの空間に見せるための仕組みについて説明します。

もし、ドメイン名空間が1つだけのゾーンで構成されていたらどうなってしまうでしょうか。ドメイン名空間全体をルートゾーンに記載し、@<tt>{www.sub.example.com}のような情報を直接登録するような運用は現実的ではありません。ルートゾーンの運用者が悲鳴を上げてしまうでしょう。
ドメイン名空間を分割できれば、管理すべき範囲を絞ることができるので編集がやりやすくなり、またそれぞれの部分を別のサーバーに持たせることによって、スケーラビリティを実現できます。

DNSではドメイン名空間を分割する方法として、サブツリーをゾーンという単位で切り出して、その運用を別の組織に委ねることを行っています。これを、ゾーンの@<kw>{委任, delegation}と呼びます。サブツリー全体を切り出しますので、委任の効果は委任する（される）ドメイン名自身とそのサブドメイン全体に及びます。

前節のコラムで、「@<tt>{com}のルート」という呼び方はふさわしくない、と説明しました。委任されたゾーンの頂点であるドメイン名（たとえば、@<tt>{com}ゾーンの中での@<tt>{com}というドメイン名）は、@<kw>{ゾーンの頂点, zone apex}と呼ばれています。

DNSでは、委任のための情報（あるドメイン名から下のサブツリーは他のゾーンに委任されていることを示す情報）もドメイン名空間にレコードの形で含まれています。具体的には、委任する側のゾーンに、委任したいドメイン名について下記の情報を記載します。

 * 委任先の権威DNSサーバーを示す@<tt>{NS}レコード
 ** つまり、委任先の情報を持つ権威DNSサーバーのドメイン名
 ** NSレコードの内容はドメイン名で書き、IPアドレスは書けない
 * 上記権威DNSサーバーの@<tt>{A}レコードや@<tt>{AAAA}レコード（必要な場合のみ）
 ** つまり、委任先の権威DNSサーバーのIPv4・IPv6アドレス

また、委任される側のゾーンには、ゾーンの頂点に下記のレコードを記載します。

 * ゾーンの頂点を表す@<tt>{SOA}レコード1つ
 ** ゾーン全体に関わるパラメーターや、ゾーン内容の変更を検知するための情報などを含む
 * 自ゾーンの権威DNSサーバーを示す@<tt>{NS}レコード
 ** つまり、自ゾーンの情報を持つ権威DNSサーバーのドメイン名
 ** 委任元に記載したものと同じ情報を記載する

@<tt>{NS}レコードは委任する側（親側）と委任される側（子側）の両方に、同じ情報が記載されています。この@<tt>{NS}レコードを持つドメイン名のことを、ゾーンの切れ目であることから@<kw>{ゾーンカット, zone cut}と呼びます。

それでは、@<tt>{example.com}ゾーンから@<tt>{sub.example.com}ゾーンを切り出して委任する場合の例について説明します。
@<tt>{sub.example.com}ゾーンを持つ権威DNSサーバーとして、@<tt>{ns1.sub.example.com}と@<tt>{ns2.example.net}の2つを指定しています。

//list[DelegationExampleParentSide][example.comゾーン（委任する側）]{
$ORIGIN example.com.
...                                      ;; ほかのドメイン名の情報
sub      IN  NS    ns1.sub.example.com.
sub      IN  NS    ns2.example.net.
ns1.sub  IN  A     192.0.2.53            ;; グルーレコード（後述）
ns1.sub  IN  AAAA  2001:db8:beef::1:53   ;; グルーレコード（後述）
//}
//list[DelegationExampleChildSide][sub.example.comゾーン（委任される側）]{
$ORIGIN sub.example.com.
@    IN  SOA   ...                   ;; ゾーンの頂点に必要な情報（省略）
@    IN  NS    ns1.sub.example.com.
@    IN  NS    ns2.example.net.
ns1  IN  A     192.0.2.53
ns1  IN  AAAA  2001:db8:beef::1:53
//}

ゾーンの頂点（@<tt>{SOA}レコードが1つ存在します）以外の場所で@<tt>{NS}レコードが現れた場合、そのドメイン名から下のサブツリーは別のゾーンとして委任されていることを表しています。

委任する側（親側）と委任される側（子側）には、全く同じ内容のNSレコードを記載すると説明しました。それらが異なっている場合、@<kw>{不適切な委任設定, lame delegation}と呼ばれる状態となり、名前解決に支障が出る場合があります。

委任されたサブツリー全体をさらに別のゾーンとして委任することはできませんが、一部のサブツリーを別のゾーンに委任することは可能です。こうして、ドメイン名空間はたくさんのゾーンで構成されています。


== グルーレコード

#@# 図がいる気がする

前節で、権威DNSサーバーのIPアドレス情報を「必要な場合のみ」と説明しました。この節では、その条件について説明します。

@<kw>{グルーレコード, glue records}とは、委任元のゾーンに記載する、委任先の権威DNSサーバーのIPアドレス（@<tt>{A}レコード・@<tt>{AAAA}レコード）のことです。

これまで、フルリゾルバーはルートヒントの情報を基に、ドメイン名空間のツリーをたどって（この動作を@<kw>{反復検索, iterative resolution}と呼びます）名前解決を行うと説明しました。
反復検索の過程で@<kw>{委任を示す応答, referral response}を受け取る（NSレコードが返ってくる）と、委任先の権威DNSサーバーに改めて問い合わせることになります。しかし、NSレコードの内容はドメイン名で記載されています。つまり、委任先に問い合わせるには、その委任先である権威DNSサーバーのドメイン名を名前解決する必要があるのです。

委任先である権威DNSサーバーのドメイン名が、その委任先ゾーンの中にある場合（これを@<kw>{内部名である権威DNSサーバー, in-domain name server}と呼びます）には、「缶切りは缶の中」状態になってしまいます。たとえば、前節の例にある@<tt>{example.com}ゾーンから@<tt>{sub.example.com}ゾーンを委任するにあたり、委任先の権威DNSサーバーのドメイン名を@<tt>{ns1.sub.example.com}にしたいとします。このとき、@<tt>{ns1.sub.example.com}というドメイン名は委任先である@<tt>{sub.example.com}ゾーンの中（もしくは、その委任先）に存在するため、NSレコードだけではそのIPアドレスを得ることができません。

このような場合、委任元のゾーン@<tt>{example.com}に委任先の権威DNSサーバー@<tt>{ns1.sub.example.com}のIPアドレスをグルーレコードとして記載します。すると、委任元の権威DNSサーバーは委任を示す応答にこのグルーレコードを含めます。これにより、フルリゾルバーは委任先の権威DNSサーバーのIPアドレスを知ることができるため、「缶切りは缶の中」問題が解決されます。

グルーレコードは、本来は委任先のゾーンが管理するべき情報であり、（ゾーンカットの先にあるため）委任元の管理が及ばない情報です。しかし、前述のような理由でドメイン名空間のツリーをたどるために必要であるため、委任元のゾーンに記載されています。ですので、委任元の権威DNSサーバーに問い合わせても直接応答されるものではなく、委任を示す応答の一部としてのみ用いられます。フルリゾルバーも、委任を示す応答の一部として返ってきた情報は、反復検索のための一時的な情報として扱います。つまり、その情報だけで名前解決の結果としてクライアントに応答することはありません。
#@# これ伏線

また、委任元にグルーレコードとして権威DNSサーバーのIPアドレス情報が必要なのは、内部名である権威DNSサーバーだけです。それ以外の場合（たとえば前節の例にある@<tt>{ns2.example.net}）には、IPアドレス情報を登録する必要はありません。

===[column] 内部名を使うべき？

内部名である権威DNSサーバーは、委任を示す応答にそのIPアドレス情報がグルーレコードとして含まれているため、反復検索の手間が少なくなります。

一方、本節で説明したとおり、グルーレコードは本来「委任先のゾーンが管理するべき情報」を、委任元に記載しています。このため、そのIPアドレスの変更にあたっては、自分の管理下にある委任先のゾーンの変更だけではなく、委任元への連絡と変更手続きが必要となってしまいます。
さらに、自分で権威DNSサーバーを運用せずにDNSホスティングサービスを利用している場合、そのIPアドレスを勝手に内部名である権威DNSサーバーとして登録してしまうと、それはサービスの提供者（権威DNSサーバーの運用者）の与り知るところではありません。サービスの提供者が権威DNSサーバーのIPアドレスを変更したとき、名前解決ができなくなるといった大変困った状態となってしまいます。

権威DNSサーバーのドメイン名を内部名にするのは、そのように設定する必要があり、そのサーバーを自分が運用している場合だけにするのが賢明です。特にDNSホスティングサービスを利用している場合、サービスの提供者が案内するとおりに委任の設定を行いましょう。

===[/column]


== よくある誤解

この節では、ゾーンと委任に関するよくある誤解について解説します。

=== 「@<tt>{.}」の切れ目で必ず委任されているわけではない

委任の節で、それでは、@<tt>{.}の切れ目で必ずゾーンが分かれている（委任されている）のでしょうか。

たとえば、アメリカ・カリフォルニア州のWebサイトである@<tt>{www.state.ca.us}について見てみます。このドメイン名は、カリフォルニア州政府によって登録されている@<tt>{state.ca.us}ゾーンに記載されています。では、@<tt>{state.ca.us}ゾーンは@<tt>{ca.us}ゾーンから委任されているのかというと、実はそうではなく、@<tt>{us}ゾーンから直接委任されているのです。つまり、@<tt>{ca.us}ゾーンは存在していません。

実際にツリーをたどって見てみましょう。まずは、ルートDNSサーバーの1つである@<tt>{a.root-servers.net}に問い合わせてみます。

//list[StateCaUsFromRoot][a.root-servers.netへの問い合わせ]{
$ dig +norec -t A www.state.ca.us @a.root-servers.net

; <<>> DiG 9.8.3-P1 <<>> +norec -t A www.state.ca.us @a.root-servers.net
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 64179
;; flags: qr; QUERY: 1, ANSWER: 0, AUTHORITY: 6, ADDITIONAL: 9

;; QUESTION SECTION:
;www.state.ca.us.		IN	A

;; AUTHORITY SECTION:
us.			172800	IN	NS	a.cctld.us.
us.			172800	IN	NS	b.cctld.us.
us.			172800	IN	NS	c.cctld.us.
us.			172800	IN	NS	e.cctld.us.
us.			172800	IN	NS	f.cctld.us.
us.			172800	IN	NS	k.cctld.us.

（略）
//}

@<tt>{AA}ビットがオフ、@<tt>{status}が@<tt>{NOERROR}で、@<tt>{ANSWER}セクションが空であることから、問い合わせたドメイン名に関する情報は委任先のゾーンに存在することを表しています。それでは、委任先である@<tt>{us}ゾーンを持つ権威DNSサーバーの1つ@<tt>{a.cctld.us}に改めて同じ内容を問い合わせてみましょう。

//list[StateCaUsFromCctldUs][a.cctld.usへの問い合わせ]{
$ dig +norec -t A www.state.ca.us @a.cctld.us

; <<>> DiG 9.8.3-P1 <<>> +norec -t A www.state.ca.us @a.cctld.us
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 3603
;; flags: qr; QUERY: 1, ANSWER: 0, AUTHORITY: 3, ADDITIONAL: 0

;; QUESTION SECTION:
;www.state.ca.us.		IN	A

;; AUTHORITY SECTION:
state.ca.us.		7200	IN	NS	ns6.net.ca.gov.
state.ca.us.		7200	IN	NS	ns5.net.ca.gov.
state.ca.us.		7200	IN	NS	ns7.net.ca.gov.

（略）
//}

@<tt>{AA}ビットがオフ（@<tt>{flags}に@<tt>{aa}の記載がない）、@<tt>{status}が@<tt>{NOERROR}で、@<tt>{ANSWER}セクションが空であることから、問い合わせたドメイン名に関する情報は委任先のゾーンに存在することを表しています。ここで、委任先のゾーンがどこかというと、@<tt>{AUTHORITY}セクションに含まれるレコードのとおり、@<tt>{state.ca.us}です。つまり、@<tt>{ca.us}というゾーンは存在していないのです。

このように、ドメイン名にある@<tt>{.}の切れ目で必ずゾーンが分かれている（委任されている）わけではありません。つまり、ドメイン名のツリー（各ノードがラベルを持ち、0個以上のレコードが存在できる）とゾーンの委任の構造とは、一致しないことがあります。
#@# これ伏線

=== ある名前に結びつくレコードが1つもないこともある

#@# まだかいてない

すべての存在するドメイン名は、レコードを持っているのでしょうか。

実は、ドメイン名空間のツリーにあるノードのうち、どのレコードも持っていないノードが存在する場合もあります。たとえば、@<tt>{example.com}ゾーンに@<tt>{www.sub.example.com}というドメイン名を持つレコードが存在しているものの、@<tt>{sub.example.com}というドメイン名を持つレコードが存在していないケースです。

#@# 図が要るかも

このような、子ノードの存在により自身がノードとして存在しているものの、レコードを持たないドメイン名のことを、@<kw>{ENT, Empty Non-Terminals}と呼びます。
#@# これ伏線


== 逆引き

この節では、IPv4アドレス・IPv6アドレスからドメイン名を検索する操作である@<kw>{逆引き, reverse lookup}について説明します。

ここまで説明してきたように、ドメイン名空間は管理者の異なるたくさんのゾーンから構成されています。このため、あるIPアドレスを内容に含むレコードの検索をしようとすると、すべてのゾーンを洗い出すことになります。これは現実的ではありません（@<tt>{IQUERY}という操作が古い規格で定義されていますが、実装されることはなく後に廃止されました）。

このためDNSの世界では、「逆引き」を下記のように定義しました。

 * IPアドレスを、逆引き用ドメイン名としてドメイン名空間の一部にマップする
 * 逆引き用ドメイン名のゾーンは、IPアドレスブロックの管理者に委任する
 * 逆引き用ドメイン名の@<tt>{PTR}レコードの内容を逆引きの結果とする

IPアドレスをドメイン名空間の一部にマップして情報を登録することで、DNSの上で扱うことができます。ただし、正引きドメイン名とIPアドレスの逆引きとは全く別のゾーンで管理されているため、それらの内容が必ず対応するわけではありません（登録可能なドメイン名の数とIPv4アドレスの数を考えると、納得できるかと思います）。また、@<tt>{PTR}レコードの内容には何でも記載することが可能です。たとえば、自分の管理していないドメイン名を記載したり、 @<tt>{127.0.0.1}のような文字列を書くこともできてしまいます。逆引きゾーンの管理者の自己申告に近いですが、悪いことを考えない限りは、でたらめな内容を書いてもいいことはありませんし、普通やりません。

それでは、IPv4アドレスとIPv6アドレスのそれぞれについて、逆引きドメイン名にどのようにマップされているか説明します。

=== IPv4アドレス
 * 1オクテットのまとまりを10進数表記して順序を逆にしたラベル4個を構成し、最後に@<tt>{in-addr.arpa}を付ける
 * 例: @<tt>{203.0.113.4}
 ** @<tt>{4.113.0.203.in-addr.arpa}
=== IPv6アドレス
 * 4ビットのまとまりを16進数表記して順序を逆にしたラベル32個を構成し、最後に @<tt>{ip6.arpa} を付ける
 * 例:  @<tt>{2001:db8:beef:feed:0123:4567:89ab:cdef}
 ** @<tt>{f.e.d.c.b.a.9.8.7.6.5.4.3.2.1.0.d.e.e.f.f.e.e.b.8.b.d.0.1.0.0.2.ip6.arpa}

IPv4アドレス・IPv6アドレスのどちらも、「順序を逆にする」という操作が出てきました。これは、IPアドレスの割り当て階層構造と、DNSにおけるゾーンの委任の階層構造が異なるためです。

IPv4アドレスもIPv6アドレスも、左側ほど大きな単位での割り当て、右側ほど細かい単位での割り当てがなされています。IPv4の例では、たとえば@<tt>{133.0.0.0/8}はIANAからAPNICを通してJPNICに割り振られ、その一部である@<tt>{133.51.0.0/16}が筑波大学に割り当てられています。
一方、ドメイン名はその逆で、左側ほど細かく（子孫側）、右側ほど大きく（親側）になっています。たとえば@<tt>{jp}はルートゾーンからJPRSに委任され、その一部である@<tt>{tsukuba.ac.jp}は筑波大学が登録し、委任されています。

逆引き用ドメイン名で順序を逆にすることで、@<tt>{133.in-addr.arpa}ゾーンをJPNICに委任し、その一部である@<tt>{51.133.in-addr.arpa}ゾーンをさらに筑波大学に委任する、といったように、IPアドレスの割り当て階層構造をゾーンの委任に揃えることができるのです。

@<tt>{in-addr.arpa}および@<tt>{ip6.arpa}のサブツリーは、ルートゾーンからIPアドレスの割り振りを管理する組織などを経由して、IPアドレスブロックの管理者に委任されています。ただし、割り当てを受けている組織のポリシーにより、逆引きドメイン名を設定していなかったり、そもそも逆引きゾーンの委任がされていない場合もあります。

なお、IPv4アドレスに関しては、@<tt>{/24}より細かい単位での割り当ても行われています。しかし上述のとおりラベルを1オクテット単位としているため、それよりも細かい単位のブロックをどのように委任するかという問題があります。本節では説明しませんが、運用上はいくつかの手段が用意されており、委任元と相談して選択することとなります。

===[column] IPv6逆引き、今と昔

IPv6アドレスの逆引きについて、昔は@<tt>{ip6.int}というゾーンも使われていました。しかし、@<tt>{ip6.arpa}に移行することとなり、2005年に発行されたRFC 4159により廃止されています。未だに@<tt>{ip6.int}と書いてある本があったら、窓から投げ捨ててよいでしょう。

===[/column]

== CNAME

サーバーに付けているドメイン名とは別に、役割を表す名前をドメイン名として使いたいときがあるでしょう。@<tt>{ci.example.net}や@<tt>{ci.example.net}を@<tt>{amnesiac.dev.example.com}のエイリアスとしておくと、サーバーのIPアドレスが変わったときでも1ヶ所のドメイン名を修正するだけで済みます。

DNSでは、このエイリアスの仕組みが用意されています。それが@<tt>{CNAME}レコードです。
ドメイン名に@<tt>{CNAME}レコードを書くと、それは「別名」であるということになります。「本名」は、@<tt>{CNAME}レコードの内容にドメイン名として記載します。別々のドメイン名に同じ「本名」を持つ@<tt>{CNAME}レコードを書くこともできます。

反復検索の過程で@<tt>{CNAME}レコードの応答を受け取ったフルリゾルバーは、その「本名」に対して改めて問い合わせします。
なお、「本名」のことを権威DNSサーバーが知っている（同じゾーンに書いてある）場合、気を利かせて@<tt>{CNAME}レコードとともにその先にある情報も合わせて応答することもできます。なんとなく委任と似ているようですが、違う点はサブツリー全体ではなくそのドメイン名だけに効果が及ぶことです。

@<tt>{CNAME}レコードには、特有の注意点があります。それは、他のタイプのレコードと共存できない@<fn>{cname-exception-dnssec}ことです。
つまり、@<tt>{CNAME}レコードが存在するドメイン名には、他のタイプのレコード（@<tt>{A}レコードや@<tt>{AAAA}レコードなど）を書くことはできません。
これは、別名を表すという特徴によるものです。もし、@<tt>{CNAME}レコードが存在するドメイン名に@<tt>{A}レコードも書いてあった場合、名前解決の結果としては、そのレコードを返すべきなのか、「本名」のほうに書いてあるレコードを返すべきなのか、曖昧になってしまうでしょう。

//list[BADExampleAnotherRRAtAlias][悪い例: 他のタイプのレコードが書いてある]{
$ORIGIN example.com.
host1  IN  A      203.0.113.4
www    IN  CNAME  host1
www    IN  A      192.0.2.80    ;; NG!
//}

他のタイプのレコードと共存できないことにより、さらに別の制限が生まれます。それは、ゾーンの頂点に@<tt>{CNAME}レコードを書くことはできない、というものです。
委任の節で説明しましたが、ゾーンの頂点には必ず@<tt>{SOA}レコード1つと@<tt>{NS}レコードが存在します。しかし、@<tt>{CNAME}レコードは他のタイプのレコードとは共存できないため、これらのレコードを記載することはできません。よって、ゾーンの頂点に@<tt>{CNAME}レコードを書くことはできないのです。無理矢理書くこともできるでしょうが、DNSに関するRFCに準拠しない状態になっています。設定時にはたまたま正しく動いても、後になって名前解決に問題が起こる可能性があるため、避けるべきでしょう。DNSホスティングサービスによっては、エイリアスを作るための別のしくみが用意されていることもあります。

//list[BADExampleAliasAtApex][悪い例: ゾーンの頂点にCNAMEが書いてある]{
$ORIGIN example.com.
@    IN  SOA   ...                              ;; ゾーンの頂点に必要な情報（省略）
@    IN  NS     ns1.example.net.
@    IN  NS     ns2.example.net.
@    IN  CNAME  cool.cdn.provider.example.net.  ;; NG!
www  IN  CNAME  cool.cdn.provider.example.net.
//}

また、内容の異なる複数の@<tt>{CNAME}レコードを同じドメイン名に書いてはいけない、という制限もあります。これは、@<tt>{CNAME}レコードの内容は「本名」であることから、「本名」が複数ある状況はおかしい、という考え方によるものです。

//list[BADExampleMultipleAlias][悪い例: 複数CNAMEが書いてある]{
$ORIGIN example.com.
www  IN  CNAME  cool.cdn.provider.example.net.      ;; NG!
www  IN  CNAME  awesome.cdn.provider.example.net.   ;; NG!
//}

もう1つ、@<tt>{NS}レコードや@<tt>{MX}レコードの内容であるドメイン名については、@<tt>{CNAME}レコードを書いてエイリアスにすることはできません。
#@#RFC 2181

//list[BADExampleNSMXPointsToAlias][悪い例: @<tt>{NS}レコードや@<tt>{MX}レコードの内容がエイリアス]{
$ORIGIN example.com.
@            IN  MX     mail          ;; NG!
mail         IN  CNAME  superserver
www          IN  CNAME  superserver
sub          IN  NS     superserver   ;; NG!
superserver  IN  A      203.0.113.4
//}

@<tt>{CNAME}レコードの指す先のドメイン名に、さらに@<tt>{CNAME}レコードが記載することはできるのでしょうか。RFCでは、このような複数段の@<tt>{CNAME}チェインを許容しています。CDNサービスを利用しているWebサイトのドメイン名で、よく見かける構成です。

ただし、複数段の@<tt>{CNAME}チェインを作る場合には注意が必要です。たとえば、@<tt>{a.example.com}の@<tt>{CNAME}レコードの指す先に@<tt>{b.example.com}を、逆に@<tt>{b.example.com}の@<tt>{CNAME}レコードの指す先に@<tt>{a.example.com}を指定すると、@<tt>{CNAME}チェインのループができてしまいます。フルリゾルバーがこのドメイン名を名前解決しようとすると、無限ループに陥ってしまうでしょう。この対策として、多くの実装では@<tt>{CNAME}をたどる回数に制限を設けています。たとえば、Unbound@<fn>{unbound}では8段までに制限されています@<fn>{unbound-max-restart-count}。

複数段の@<tt>{CNAME}チェインはRFCで許容されているものの、ループの問題や、フルリゾルバーでの名前解決の手間が増えることを考えると、多くとも2〜3段くらいにとどめておくのがよいでしょう。

本節では説明しませんが、特定のドメイン名以下のサブツリーをまるごと別のサブツリーにリダイレクトする@<tt>{DNAME}レコードも定義されています。

//footnote[cname-exception-dnssec][DNSSECのためのレコードタイプについては、例外とされています]
//footnote[unbound][@<tt>{https://www.unbound.net/}]
//footnote[unbound-max-restart-count][@<tt>{https://github.com/NLnetLabs/unbound/blob/release-1.7.0/iterator/iterator.h#L59}]

== あとがき

本章では、ゾーンと委任という切り口でDNSについて説明しました。また、逆引きはDNSでどのように実現されているのか、@<tt>{CNAME}によるエイリアスの仕組みについて説明しました。
DNSにはこれ以外にも、キャッシュの仕組み、不在応答、ワイルドカードの扱いといった厄介なものたちが存在します。本章では全く触れませんでしたが、これらはDNSにデータの出自の認証と完全性の保証を導入する@<kw>{DNSSEC, DNS Security Extensions}を、その落とし穴も含めて理解するために必要となります。また機会があれば、説明したいと思います。
