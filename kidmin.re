= エンジニアのためのDNS（再）入門（中級者編）（仮）

== はじめに

前章では初級者編として、DNSを構成する要素（ドメイン名空間・権威DNSサーバー・フルリゾルバー）について紹介しました。
本章では、ドメイン名空間について、ゾーンと委任という切り口でもう少し詳しく説明します。

== ゾーン

ドメイン名空間は、各ノードにラベルを持つツリー構造になっています。

 * ドメイン名空間の一部
 ** サブツリーを構成するレコードのまとまり


== ルートゾーン

@<kw>{ルートゾーン, root zone}とは、ドメイン名空間を構成するツリーの根（root）となるゾーンです。
ドメイン名空間は、ルートゾーンを根として持ち、その子孫であるたくさんのゾーンから構成されています。

ルートゾーンの管理に関するポリシーの策定は@<kw>{ICANN, Internet Corporation for Assigned Names and Numbers}によって行われています。実際の運用は@<kw>{IANA機能運用者, IANA Functions Operator}として選ばれた@<kw>{PTI, Public Technical Identifiers}が行っています。

ルートゾーンをサービスする権威DNSサーバー（本稿執筆時点では@<tt>{[a-m].root-servers.net}）のことを、@<kw>{ルートサーバー, root server}または@<kw>{ルートDNSサーバー}と呼びます。ルートDNSサーバーは、複数のオペレーターによって運用されています。詳しくは、@<tt>{http://www.root-servers.org/}を参照して下さい。

全てのフルリゾルバーは、ルートDNSサーバーについての情報を持っています。
具体的には、@<kw>{ルートヒント, root hints}と呼ばれる下記の情報を持っています。これらは、ルートゾーン自身にも含まれています。

 * ルートゾーンのNSレコード
 ** つまり、ルートDNSサーバーのホスト名
 * 上記ホストのAレコードおよびAAAAレコード
 ** つまり、ルートDNSサーバーのIPv4・IPv6アドレス

フルリゾルバーは、このルートヒントの情報とともに配布されています。ルートヒントに記載されているサーバーを起点に、ドメイン名空間のツリーをルートからたどって、名前解決を行っています。
最新のルートヒントの情報は、@<tt>{https://www.iana.org/domains/root/files}にて公開されています。

なお、ルートヒントの情報を直接使うのではなく、最新の情報を使うために、ルートゾーンに含まれるルートヒント情報を取得し直す実装もあります。この取り直す動作のことを、@<kw>{プライミング, priming}と呼びます。

たまに、@<tt>{com}ゾーンを持つ権威DNSサーバー（本稿執筆時点では@<tt>{[a-m].gtld-servers.net}）のことを「@<tt>{com}のルート」と呼ぶ人もいます。しかし前述の通り、DNS名前空間は1つのツリーからなっているので、「ルート」とはその頂点を指すことになります。このため、それ以外の部分をルートと呼ぶことはふさわしくありません。


== 委任

ドメイン名空間全体をルートゾーンに記述し、@<tt>{www.sub.example.com}のような情報を直接登録するような運用は現実的ではありません。ルートゾーンの運用者が悲鳴を上げてしまうでしょう。

サブツリーの運用を別の組織に委ねることで行っています。これを、@<kw>{委任, delegation}と呼びます。

 * 委任先のNSレコード
 ** つまり、委任先の情報を持つ権威DNSサーバーのホスト名
 * 上記のAレコードおよびAAAAレコード（必要な場合のみ）
 ** つまり、委任先の権威DNSサーバーのIPv4・IPv6アドレス（@<kw>{グルー, glue}と呼ぶ）

@<tt>{example.com}ゾーンから@<tt>{sub.example.com}ゾーンを切り出す場合の例について説明します。
@<tt>{sub.example.com}ゾーンを持つ権威DNSサーバーとして、@<tt>{ns1.sub.example.com}と@<tt>{ns2.example.net}の2つを指定しています。

//list[DelegationExampleParentSide][example.comゾーン（委任する側）]{
sub.example.com.      IN  NS    ns1.sub.example.com.
sub.example.com.      IN  NS    ns2.example.net.
ns1.sub.example.com.  IN  A     192.0.2.53
ns1.sub.example.com.  IN  AAAA  2001:db8:beef::1:53
//}
//list[DelegationExampleChildSide][sub.example.comゾーン（委任される側）]{
sub.example.com.      IN  SOA   ...                   ;; ゾーンの頂点に必要な情報（省略）
sub.example.com.      IN  NS    ns1.sub.example.com.
sub.example.com.      IN  NS    ns2.example.net.
ns1.sub.example.com.  IN  A     192.0.2.53
ns1.sub.example.com.  IN  AAAA  2001:db8:beef::1:53
//}

サブツリーのポリシー（登録要件、費用など）は、その運用組織ごとに異なります。


== グルー

前節で、権威DNSサーバーのIPアドレス情報を「必要な場合のみ」と記載しました。この節では、その条件について説明します。


== よくある誤解

この節では、ゾーンと委任に関するよくある誤解について解説します。

=== 「@<tt>{.}」の切れ目が必ず委任を表してはいないことに注意

委任の節で、それでは、@<tt>{.}の切れ目で必ずゾーンが分かれている（委任されている）のでしょうか。

たとえば、アメリカ・カリフォルニア州のWebサイトである@<tt>{www.state.ca.us}について見てみます。このドメイン名は、カリフォルニア州政府によって登録されている@<tt>{state.ca.us}ゾーンに記載されています。では、@<tt>{state.ca.us}ゾーンは@<tt>{ca.us}ゾーンから委任されているのかというと、実はそうではなく、@<tt>{us}ゾーンから直接委任されているのです。つまり、@<tt>{ca.us}ゾーンは存在していません。

実際にツリーをたどって見てみましょう。まずは、ルートDNSサーバーの1つである@<tt>{a.root-servers.net}に問い合わせてみます。

//list[StateCaUsFromRoot][ルートDNSサーバーへの問い合わせ]{
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

@<tt>{status}が@<tt>{NOERROR}で、@<tt>{ANSWER}セクションが空であることから、問い合わせたドメイン名に関する情報は委任先のゾーンに存在することを表しています。ここで、委任先のゾーンがどこかというと、@<tt>{AUTHORITY}セクションに記載のリソースレコードのとおり、@<tt>{state.ca.us}です。

このように、ドメイン名にある@<tt>{.}の切れ目で必ずゾーンが分かれている（委任されている）わけではありません。

=== ある名前に結びつくリソースレコードが1つもないこともある

 * @<tt>{example.com} ゾーンの中で、
    @<tt>{www.sub.example.com} という名前を持つリソースレコードが存在しているが、
    @<tt>{sub.example.com} という名前を持つリソースレコードが存在していないケース
 * これが @<kw>{ENT, Empty Non-Terminal}


== 逆引き

この節では、IPv4アドレス・IPv6アドレスからドメイン名を検索する操作である@<kw>{逆引き, reverse lookup}について説明します。

ここまで説明してきたように、ドメイン名空間は管理者の異なる複数のゾーンから構成されています。このため、あるIPアドレスに対応するドメイン名を検索しようとすると、すべてのゾーンからIPアドレスを洗い出すことになります。これは現実的ではありません。

このため、（IPアドレスをドメイン名としてドメイン名空間にマップ、PTR検索する）

 * @<tt>{A} レコードや @<tt>{AAAA} レコードとして登録されている情報を直接参照しているわけではない
 * IPアドレスをドメイン名にマップして、その名前の @<tt>{PTR} レコードを参照している
 ** 逆引きに関するDNSでの約束
 ** ドメイン名空間にあるドメイン名からレコードを検索するという操作は正引きと同じ
 * 仕組み上、 @<tt>{A} レコードや @<tt>{AAAA} レコードと対応付けられているわけではない
 ** 何でも書ける
 ** IPアドレス管理者の自己申告に近いが、でたらめなことを書いてもいいことがないので普通やらない
 * IPv4
 ** 1octetずつ逆にして10進数表記してラベル4個を構成し、最後に @<tt>{in-addr.arpa} を付ける
 ** 例:  @<tt>{203.0.113.4} → @<tt>{4.113.0.203.in-addr.arpa}
 ** @<b>{発展課題} prefixが/24より長いCIDRブロックの逆引き
 * IPv6
 ** 4bitずつ逆にして16進数表記してラベル32個を構成し、最後に @<tt>{ip6.arpa} を付ける
 ** 例:  @<tt>{2001:db8:beef:feed:0123:4567:89ab:cdef} → @<tt>{f.e.d.c.b.a.9.8.7.6.5.4.3.2.1.0.d.e.e.f.f.e.e.b.8.b.d.0.1.0.0.2.ip6.arpa}
 ** 昔は @<tt>{ip6.int} というゾーンも使われていたが廃止済; 本に書いてあったら投げ捨ててよい


== DNSのめんどくさいものたち

書くか落とすか微妙なところ

=== ワイルドカード

=== CNAME

=== DNAME

