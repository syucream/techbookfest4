= 海賊版サイトのブロッキングに関する問題点 [draft]

海賊版対策として、政府が特定のWebサイトを名指ししてブロッキングすることが適当である
という方針を示しました@<fn>{kantei-blocking}。
名指しされたサイトが閉鎖されるなど状況が流動的ですが、問題点について整理してみました。

//footnote[kantei-blocking][@<tt>{https://www.kantei.go.jp/jp/singi/hanzai/kettei/180413/kaizokuban_1.pdf}]

== 何が起きているの？

4月13日、政府から、海賊版サイトのブロッキングに関する対策が公表されました。
具体的なサイトの名前が「ブロッキング対象ドメイン」として挙げられています。

対象がドメイン（名）であることから、もし仮に実装されるとしたら、
ISPが提供するフルリゾルバーで対処されることになるでしょう。
具体的には、ブロックされていることを通知するWebページを表示するために、
別のIPアドレスを返すようにすることが考えられます。

//image[paper_img01][ブロッキングが行われた場合][scale=0.85]

== 何が問題なの？

=== 技術的な観点

DNSブロッキングを行う対象はドメイン名です。ころころ変えられてしまえば、リストの維持にも
コストがかかることになります。
対象とされたWebサイトの一部は閉鎖されてしまったようですが、名前を変えて、
またはドメイン名を変えて再開されることもあるかもしれません。

また、ブロッキングを行っている旨を表示するWebサイトを用意して、そのIPアドレスを返す場合、
HTTPSのサイトだと証明書に関する警告が表示されてしまいます。

（ドメイン名でブロックするというけど、インターネットは80/tcpと443/tcpだけじゃないよね？
うっかり25/tcpまで吸い込んでしまうと、通信をねじ曲げるという点でより深刻な問題が起きてしまう）

ISPが提供するフルリゾルバーにてブロッキングを行うことになりますが、
それ以外の手段で名前解決をするように設定してしまえば、ブロッキングを回避することは
そう難しいことではありません。

（hostsに書く、1.1.1.1を使う、があるが、具体的に書くと問題がありそうで怖いので具体例は明示しない）
（「ブロッキングを突破、くりっくなう」的な怪しい何かでマルウェア感染、というシナリオは関連性が薄いのと
話がややこしくなるのでここでは触れない）

=== ポリシー的な観点

今回は「緊急避難」という形で政府からブロッキングを行うことが示されました。
ブロッキングという形で緊急避難を適用するとしたら、得られる効果（著作権の保護）と失われるもの
（ブロッキング対象との通信であるか否かにかかわらず、一律に通信の秘密を侵す）とのバランスや、
他に手段がないのか、といった点について、本来であれば政府の意見だけではなく、通信業界や
一般のISP加入者も含めて検討する必要があるでしょう。

またブロッキングを行うにあたり、「このドメイン名をブロックする」というリストを誰が管理するのかは
大きな問題になります。リストにどのドメイン名を載せるのか、その基準をどのように決めるのかについては、
緊急避難を適用するのであれば、慎重に決める必要があるでしょう。決める過程で政府の関与を許してしまうと、
それは政府による検閲につながる恐れがあります。今回のケースでは政府から特定の3つのWebサイトが
示されましたが、どのように3つを決めたのか、他のWebサイトを追加するとしたらその基準は何か、
という点をはっきりさせる必要があるでしょう。


== 今もやっていることと何が違うの？

DNSブロッキングを行うことは、通信の一環として行われる名前解決の過程において、
その情報を知得（のぞき見て）・窃用（ブロッキングするという本来の目的以外に使う）することから、
電気通信事業者に課せられている通信の秘密の保護を侵すことになります。

=== マルウェア対策

マルウェアを配布しているWebサイトについて、DNSブロッキングを行っているISPがあります。

（これはどう整理されている？ちょっとわからない）
（https://www.jaipa.or.jp/other/mtcs/guideline_v4.pdf#page=12 の例は、マルウェアが感染した後に
攻撃対象のドメイン名への応答をねじ曲げる、設備を守るための緊急避難の話。なので、
そもそもマルウェアの感染源に接続させる仕組みとは違う: 「緊急避難」でマルウェア対策は無理筋）
（契約時の説明に書いてあるからOK？最近ISP契約していないのでそのような説明があるのかどうか分からない
契約時に「マルウェアブロッキングします」という同意を得ているのであれば、「正当行為」として
違法ではなくなる）

また、ブロッキングをされたくない場合にはオプトアウトする方法が用意されています。

ブロッキング対象のリストは民間の事業者によって管理されており、政府の関与が及ばない形になっています。

=== 児童ポルノ対策

児童ポルノに関しては、被害者である児童の人権を保護する観点から、検討が進められてきました。

もとは政府の要請ですが、ISPの業界団体で複数名の弁護士も交えたワーキンググループが組織されました。
JANOGでもブロッキングに関するセッションが開かれています@<fn>{janog26-blocking}。

検討された結果は公表され、DNSブロッキングを行うには「緊急避難」を援用するしかないこと、
その適用に関する課題の検討と整理が行われ、それに基づいてブロッキングが実施されています。

このブロッキングにはマルウェア対策のときと違ってオプトアウトする手段は用意されていません。
ですので、「緊急避難」として妥当な範囲を超えないよう、対象の管理には十分な注意が必要です。

ブロッキング対象のリストは民間の事業者によって管理されており、政府の関与が及ばない形になっています。

//footnote[janog26-blocking][@<tt>{https://www.janog.gr.jp/meeting/janog26/program/blocking.html}]

=== 今回の「緊急避難」とは？

過去に緊急避難を適用した例では、通信の秘密を侵すことの重さから、通信業界の関係者、
法律の専門家も含めて話し合いが行われてきました。
今回は、政府が「緊急避難であるため違法ではない」という見解を示しましたが、それだけです。

ブロッキングについて「民間事業者による自主的な取組として」行うことが適当とされているものの、
先の児童ポルノ対策のような検討をどう進めるか、具体的なドメイン名をリストとして作成するのは誰か、
といった点について具体的に決まっていません。

（政府が「海賊版サイトで権利がたいへんだから何か検討しません？」→業界団体でWGつくる→
  緊急避難に該当するくらい悪質で手に負えないの基準を（民間で）つくる→いろんな場所で
  「これってどうよ？」と話してみて意見集約して反映させる→じゃあやってみましょうか、
  くらいの流れなら、児童ポルノ対策とだいたい同じだし、まあ文句は出ないか）

なお、児童ポルノへのブロッキング適用を検討していたときには、著作権の侵害で緊急避難を適用するのは
無理であるという見解が示されていました@<kw>{internet-watch}。
違う結論を導くのであれば、取り巻く状況の変化を踏まえて再度同じように検討する必要があるでしょう。

//footnote[internet-watch][@<tt>{https://internet.watch.impress.co.jp/docs/event/1117888.html}]


== まとめ

海賊版Webサイトへの対策として、政府は緊急避難という解釈を示してブロッキングを要請しています。
しかし、ブロッキング対象を政府が決めていること、ブロッキングの適用に関して過去の事例のような
十分な議論が行われていないことは問題でしょう。著作権者の権利を守るためにブロッキングが必要という
意見は理解できますが、これまでに議論を積み重ねて行ってきたことを政府の一存でひっくり返すことは、
将来に影響を及ぼすことになるでしょう。政府による検閲に繋がりかねないセンシティブな問題を含むため、
必要なら立法措置などまっとうな手続きを経て、ブロッキングを行うべきでしょう。

== 宣伝

本ペーパーに関する原稿が掲載された「あかるいきんようび。〜暗号通貨とDNSのご本。〜」は Kindle Store で 2018 年 4 月 23 日より配信予定です。
本稿では前提知識として扱っている DNS に関する易しい解説記事などもありますので、もしご興味を持っていただけましたらぜひ以下の QR コードよりご購入いただけると幸いです。

//indepimage[qr_kindlestore]
