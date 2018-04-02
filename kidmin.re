= エンジニアのためのDNS（再）入門（中級者編）（仮）

== はじめに

前章では初級者編として、DNSを構成する要素（ドメイン名空間・権威DNSサーバー・フルリゾルバー）について紹介しました。
本章では、ドメイン名空間について、ゾーンと委任という切り口でもう少し詳しく説明します。

== ゾーン

#@# 説明がぬるい

ドメイン名空間は、各ノードにラベルを持つツリー構造になっています。

 * ドメイン名空間の一部
 ** サブツリーを構成するレコードのまとまり

@<kw>{リソースレコード, resource records}（以下レコード）

== ルートゾーン

@<kw>{ルートゾーン, root zone}とは、ドメイン名空間を構成するツリーの根（root）となるゾーンです。
ドメイン名空間は、ルートゾーンを根として持ち、その子孫であるたくさんのゾーンから構成されています。

全てのフルリゾルバーは、ルートDNSサーバーについての情報を持っています。
具体的には、@<kw>{ルートヒント, root hints}と呼ばれる下記の情報を持っています。これらは、ルートゾーン自身にも含まれています。

 * ルートゾーンのNSレコード
 ** つまり、ルートDNSサーバーのホスト名
 * 上記ホストの@<tt>{A}レコードおよび@<tt>{AAAA}レコード
 ** つまり、ルートDNSサーバーのIPv4・IPv6アドレス

フルリゾルバーは、このルートヒントの情報とともに配布されています。ルートヒントに記載されているサーバーを起点に、ドメイン名空間のツリーをルートからたどって、名前解決を行っています。
最新のルートヒントの情報は、@<tt>{https://www.iana.org/domains/root/files}にて公開されています。

なお、ルートヒントの情報を直接使うのではなく、最新の情報を使うために、ルートゾーンに含まれるルートヒント情報を取得し直す実装もあります。この取り直す動作のことを、@<kw>{プライミング, priming}と呼びます。

ルートゾーンの管理に関するポリシーの策定は@<kw>{ICANN, Internet Corporation for Assigned Names and Numbers}によって行われています。実際の運用は@<kw>{IANA機能運用者, IANA Functions Operator}として選ばれた@<kw>{PTI, Public Technical Identifiers}が行っています。

ルートゾーンをサービスする権威DNSサーバー（本稿執筆時点では@<tt>{[a-m].root-servers.net}）のことを、@<kw>{ルートサーバー, root servers}または@<kw>{ルートDNSサーバー}と呼びます。ルートDNSサーバーは、複数のオペレーターによって運用されています。詳しくは、@<tt>{http://www.root-servers.org/}を参照して下さい。

たまに、@<tt>{com}ゾーンを持つ権威DNSサーバー（本稿執筆時点では@<tt>{[a-m].gtld-servers.net}）のことを「@<tt>{com}のルート」と呼ぶ人もいます。しかし前述の通り、DNS名前空間は1つのツリーからなっているので、「ルート」とはその頂点を指すことになります。このため、それ以外の部分をルートと呼ぶことはふさわしくありません。


== 委任

#@# 説明がぬるい

ドメイン名空間全体をルートゾーンに記述し、@<tt>{www.sub.example.com}のような情報を直接登録するような運用は現実的ではありません。ルートゾーンの運用者が悲鳴を上げてしまうでしょう。

ドメイン名の登録者がそれぞれの範囲で情報を管理するには、ドメイン名空間を分割する仕組みが必要です。DNSでは、サブツリーの運用を別の組織に委ねることで行っています。これを、@<kw>{委任, delegation}と呼びます。

 * 委任先のNSレコード
 ** つまり、委任先の情報を持つ権威DNSサーバーのドメイン名
 ** NSレコードの内容はドメイン名で書き、IPアドレスは書けない
 * 上記の@<tt>{A}レコードおよび@<tt>{AAAA}レコード（必要な場合のみ）
 ** つまり、委任先の権威DNSサーバーのIPv4・IPv6アドレス

@<tt>{example.com}ゾーンから@<tt>{sub.example.com}ゾーンを切り出す場合の例について説明します。
@<tt>{sub.example.com}ゾーンを持つ権威DNSサーバーとして、@<tt>{ns1.sub.example.com}と@<tt>{ns2.example.net}の2つを指定しています。

//list[DelegationExampleParentSide][example.comゾーン（委任する側）]{
sub.example.com.      IN  NS    ns1.sub.example.com.
sub.example.com.      IN  NS    ns2.example.net.
ns1.sub.example.com.  IN  A     192.0.2.53            ;; グルーレコード
ns1.sub.example.com.  IN  AAAA  2001:db8:beef::1:53   ;; グルーレコード
//}
//list[DelegationExampleChildSide][sub.example.comゾーン（委任される側）]{
sub.example.com.      IN  SOA   ...                   ;; ゾーンの頂点に必要な情報（省略）
sub.example.com.      IN  NS    ns1.sub.example.com.
sub.example.com.      IN  NS    ns2.example.net.
ns1.sub.example.com.  IN  A     192.0.2.53
ns1.sub.example.com.  IN  AAAA  2001:db8:beef::1:53
//}

サブツリーのポリシー（登録要件、費用など）は、その運用組織ごとに異なります。委任されたサブツリー全体をさらに別のゾーンとして委任することはできませんが、一部のサブツリーを別のゾーンに委任することは可能です。

前節で、「@<tt>{com}のルート」という呼び方はふさわしくない、と説明しました。委任されたゾーンの頂点であるドメイン名（たとえば、@<tt>{com}ゾーンの中での@<tt>{com}というドメイン名）は、@<kw>{ゾーンの頂点, zone apex}と呼ばれています。


== グルーレコード

#@# 図がいる気がする

前節で、権威DNSサーバーのIPアドレス情報を「必要な場合のみ」と説明しました。この節では、その条件について説明します。

@<kw>{グルーレコード, glue records}とは、委任元のゾーンに記載する、委任先の権威DNSサーバーのIPアドレス（@<tt>{A}レコード・@<tt>{AAAA}レコード）のことです。

これまで、フルリゾルバーはルートヒントの情報を基に、ドメイン名空間のツリーをたどって（この動作を@<kw>{反復検索, iterative resolution}と呼びます）名前解決を行うと説明しました。
反復検索の過程で委任を示す応答を受け取る（NSレコードが返ってくる）と、委任先の権威DNSサーバーに改めて問い合わせることになります。しかし、NSレコードの内容はドメイン名で記載されています。つまり、委任先に問い合わせるには、その委任先である権威DNSサーバーのドメイン名を名前解決する必要があるのです。

委任先である権威DNSサーバーのドメイン名が、その委任先ゾーンの中にある場合には、「缶切りは缶の中」状態になってしまいます。たとえば、@<tt>{example.com}ゾーンから@<tt>{sub.example.com}ゾーンを委任するにあたり、委任先の権威DNSサーバーのドメイン名を@<tt>{ns1.sub.example.com}にしたいとします。このとき、@<tt>{ns1.sub.example.com}というドメイン名は委任先である@<tt>{sub.example.com}ゾーンの中（もしくは、その委任先）に存在するため、NSレコードだけではそのIPアドレスを得ることができません。

このような場合、委任元のゾーン@<tt>{example.com}に委任先の権威DNSサーバー@<tt>{ns1.sub.example.com}のIPアドレスをグルーレコードとして記載します。すると、委任元の権威DNSサーバーは委任を示す応答にこのグルーレコードを含めます。これにより、フルリゾルバーは委任先の権威DNSサーバーのIPアドレスを知ることができるため、「缶切りは缶の中」問題が解決されます。

グルーレコードは、実際には委任先のゾーンが管理するべき情報です。しかし、前述のような理由でドメイン名空間のツリーをたどるために必要であるため、委任元のゾーンに記載されています。ですので、委任元の権威DNSサーバーに問い合わせても直接応答されるものではなく、@<kw>{委任を示す応答, referral response}の一部としてのみ用いられます。フルリゾルバーも、委任を示す応答の一部として返ってきた情報は、反復検索のための一時的な情報として扱います。つまり、その情報だけで名前解決の結果としてクライアントに応答することはありません。

また、委任元にグルーレコードとして権威DNSサーバーのIPアドレス情報が必要なのは、権威DNSサーバーのドメイン名が委任先のゾーン（もしくは、その委任先）にある場合だけです。それ以外の場合には、権威DNSサーバーのIPアドレス情報を登録する必要はありません。


== よくある誤解

この節では、ゾーンと委任に関するよくある誤解について解説します。

=== 「@<tt>{.}」の切れ目が必ず委任を表してはいないことに注意

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

@<tt>{status}が@<tt>{NOERROR}で、@<tt>{ANSWER}セクションが空であることから、問い合わせたドメイン名に関する情報は委任先のゾーンに存在することを表しています。それでは、委任先である@<tt>{us}ゾーンを持つ権威DNSサーバーの1つ@<tt>{a.cctld.us}に改めて同じ内容を問い合わせてみましょう。

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

@<tt>{status}が@<tt>{NOERROR}で、@<tt>{ANSWER}セクションが空であることから、問い合わせたドメイン名に関する情報は委任先のゾーンに存在することを表しています。ここで、委任先のゾーンがどこかというと、@<tt>{AUTHORITY}セクションに含まれるレコードのとおり、@<tt>{state.ca.us}です。つまり、@<tt>{ca.us}というゾーンは存在していないのです。

このように、ドメイン名にある@<tt>{.}の切れ目で必ずゾーンが分かれている（委任されている）わけではありません。ゾーンが同じか分かれているかは、権威DNSサーバーに問い合わせてみないと分かりません。

=== ある名前に結びつくレコードが1つもないこともある

#@# まだかいてない

すべての存在するドメイン名は、レコードを持っているのでしょうか。

実は、ドメイン名空間のツリーにあるノードのうち、どのレコードも持っていないノードが存在する場合もあります。たとえば、@<tt>{example.com}ゾーンに@<tt>{www.sub.example.com}というドメイン名を持つレコードが存在しているものの、@<tt>{sub.example.com}というドメイン名を持つレコードが存在していないケースです。

#@# 図が要るかも

このような、ドメイン名として存在しているもののレコードを持たないドメイン名のことを、@<kw>{ENT, Empty Non-Terminals}と呼びます。


== 逆引き

この節では、IPv4アドレス・IPv6アドレスからドメイン名を検索する操作である@<kw>{逆引き, reverse lookup}について説明します。

ここまで説明してきたように、ドメイン名空間は管理者の異なるたくさんのゾーンから構成されています。このため、あるIPアドレスを内容に含むレコードの検索をしようとすると、すべてのゾーンを洗い出すことになります。これは現実的ではありません（@<tt>{IQUERY}という操作が古い規格で定義されていますが、実装されることはなく後に廃止されました）。

このためDNSの世界では、「逆引き」を下記のように定義しました。

 * IPアドレスを、逆引き用ドメイン名としてドメイン名空間の一部にマップする
 * 逆引き用ドメイン名のゾーンは、IPアドレスブロックの管理者に委任する
 * 逆引き用ドメイン名の@<tt>{PTR}レコードの内容を逆引きの結果とする

ドメイン名空間の一部にマップして情報を登録することで、DNSの上で扱うことができます。ただし、正引きドメイン名とIPアドレスとは、必ず対応するわけではありません（登録可能なドメイン名の数とIPv4アドレスの数を考えると、納得できるかと思います）。また、@<tt>{PTR}レコードの内容には何でも記載することが可能です。たとえば、自分の管理していないドメイン名を記載したり、 @<tt>{127.0.0.1}のような文字列を書くこともできてしまいます。逆引きゾーンの管理者の自己申告に近いですが、悪いことを考えない限りは、でたらめな内容を書いてもいいことはありませんし、普通やりません。

それでは、IPv4アドレスとIPv6アドレスのそれぞれについて、逆引きドメイン名にどのようにマップされているか説明します。

=== IPv4
 * 1オクテットずつ逆にして10進数表記してラベル4個を構成し、最後に@<tt>{in-addr.arpa}を付ける
 * 例: @<tt>{203.0.113.4}
 ** @<tt>{4.113.0.203.in-addr.arpa}
=== IPv6
 * 4ビットずつ逆にして16進数表記してラベル32個を構成し、最後に @<tt>{ip6.arpa} を付ける
 * 例:  @<tt>{2001:db8:beef:feed:0123:4567:89ab:cdef}
 ** @<tt>{f.e.d.c.b.a.9.8.7.6.5.4.3.2.1.0.d.e.e.f.f.e.e.b.8.b.d.0.1.0.0.2.ip6.arpa}

@<tt>{in-addr.arpa}および@<tt>{ip6.arpa}は、ルートゾーンからIPアドレスの割り振りを管理する組織などを経由して、IPアドレスブロックの管理者に委任されています。IPアドレスによっては、そもそも逆引きゾーンの登録がされていない場合もあります。

IPv6アドレスの逆引きについて、昔は@<tt>{ip6.int}というゾーンも使われていました。しかし、@<tt>{ip6.arpa}に移行することとなり、2005年に発行されたRFC 4159により廃止されています。未だに@<tt>{ip6.int}と書いてある本があったら、窓から投げ捨ててよいでしょう。


== あとがき

本章では、ゾーンと委任という切り口でDNSについて説明しました。また、逆引きはDNSでどのように実現されているのか説明しました。
DNSにはこれ以外にも、キャッシュの仕組み（特に不在応答）、ワイルドカードの扱い、CNAME・DNAMEといった厄介なものたちが存在します。これらは、本章では全く触れませんでしたが、DNSにデータの出自の認証と完全性の保証を導入する@<kw>{DNSSEC, DNS Security Extensions}を理解するために必要となります。また機会があれば、説明したいと思います。
