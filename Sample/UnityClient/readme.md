*Unity Client

**���**

�����带 ���� ����� �񵿱� �Լ��� �̿��� ��� �ΰ��� ������ �񵿱� �Լ��� �̿� �߽��ϴ�.
�α��� �� ������ �̵��� ����ȭ �ϴ� ���ر��� �Ϸ��� ������ �̰� ���ϴ� ���� �̰� �� ���� ���ٴ� ��п� �����Ͽ� �������� �ٿ��ִ� ���ر��� ������ �߷����� �ø��ϴ�.
���� ������ ���̿� ���� ����صξ��� ������ �ִٸ� ������ ���� ��Ź �帳�ϴ�.



**�ֿ� Ŭ���� ���**

NetManager_V2 : ������ �ϳ��� ������ ��� �մϴ�. ���⿡ connect, recvBuffer,sendBuffer�� �ֽ��ϴ�.
LoginNetManager : ���� �������� ������ ���� ��� ���հ����� �� �κ��Դϴ�. (���� ������ �ϳ�)
�������� NetManager_V2�� ����Ʈ�� �����ϸ� ����Ƽ ���� ��� �� ��ü�� Update()���� Update()�� ȣ�� ����� �մϴ�.
������ ���� ��û �޼����� �� Ŭ������ �޼ҵ�� �����Ͽ��� ������ (���������� ���� ��� ����)
OOP�� �°� �����ϴ� ���� Ȥ�� ��Ȱ�� ���� �и��Ͽ� �����ϴ°��� �� �����ϴ�. 

CommonData : �α��� ��û, ���̵� �ߺ� Ȯ�� ��û ��Ŷ�� ���� ��Ŷ ����ü�� �����Ǿ� �ֽ��ϴ�. 
������ ��� ��ü ��û�� ���� ��� �� int iResult�� ����� ������ �ڽĵ�� �̷���� �ֽ��ϴ�.

CommonProtocol : UINT16���ε� ���������� ���ǵǾ� �ֽ��ϴ�.

JPacket : CGSFJSON��Ŷ�� ���ǵǾ� �ֽ��ϴ�. ������ ������ ����� ������ �����Ƿ� ����� �����Ͻ� ��� ���� ���ֽñ� �ٶ��ϴ�..
{//12
    public UInt16 Protocol;//2
    public UInt32 PacketOption;//4
    public UInt32 DataCRC;//4
    public UInt16 DataSize;//2
    public const int HEADERSIZE = 12;
}

BaseScene : Mono�� ��ӹ��� Scene��ü �Դϴ�. ��� ���� ��� �޾Ƽ� �����ϵ��� �Ǿ��ְ� ���������� �޼����� �޾Ƽ� ó���ؾ��ϴ� ���� �ִٸ� ���⼭ �����մϴ�. 
(��Ʈ �����.. ä�� �޼��������..)
ConnectScene : ���������� �������� ���� �� �Դϴ�.(�ڼ��� ���� �Ʒ� ����)

Clog : ����ŵ� �˴ϴ�. debug.log�� ȭ�鿡 ����ֱ����� ��Ȱ�Դϴ�.


**���� �ʱ�ȭ�� ������ ����**

void Awake()
17�� Receive�� ���� �Լ��� �����մϴ�. ���� ��ϵ� �Լ� ���� LoginNetManager 59�ٿ� ���� �ɸ��µ� 
KeepAlive(),�Ǵ� heartbeat()�� ������ �������� ������ �ʿ䰡 ���� ��� ���⼭ �ɷ��ָ� �˴ϴ�.


void Update()
25�� LoginNetManager.I.Update(); ���ۿ� ����ִ� ���� �ִ��� �˻��Ͽ� ó���մϴ�. �̺κ��� ������ ��Ŷ�� �޾Ƶ� ó���� ���� ���մϴ�.

void OnDestory()
29�� ������ �����ݴϴ�.
30�� LoginNetManager�� �޼��� ó�� �κ��� �����ݴϴ�. 



**�޼��� ��û**

LoginNetManager.I.REQL_Login(sId, spwd);
LoginNetManager.I.REQL_IDisValid(sReg_id);
LoginNetManager.I.REQL_Registration(this.sReg_id, sReg_pwd, sReg_email, "UNITY", SystemInfo.deviceUniqueIdentifier);

����� 2�� �׸񿡼� ����Ͽ��� ���⼭�� LoginNetManager���� ��û �޼��� �ۼ����� ��������� �� �� ������ ���α׷��� �ȴٸ� �и��ϼž� �մϴ�..


**�޼��� ����**

3�� �׸��� Awake()���� ����� Onreceive()�Լ��� ������ ���ƿɴϴ�.
switch (packet.Protocol)
{
    case CommonProtocol.KEEPALIVE:

        break;
    case CommonProtocol.RES_LOGIN:
        res_Login(packet);
        break;
    case CommonProtocol.RES_IDISIDVALID:
        res_IdIsValid(packet);
        break;
    case CommonProtocol.RES_REGISTRATION:
        res_Registration(packet);
        break;
    case CommonProtocol.RES_DISCONNECTED:
        break;
}
������ ���� ���� ���ֽñ� �ٶ��ϴ�.


**�׽�Ʈ ���**

���� �ּ� 127.0.0.1:25251
���� : UnityServer������ BIN���� �̵�, UnitySimpleLoginServer.exe������ VSProject/32�� �Űܼ� ����.
Ŭ�� : ���� Asset������ ������Ʈ�� �����ϰ� ���� �ε��Ͽ� ���� �ϰų� UnityClient������ BIN������ �̵�. UnitySample�� ����.

����� LOGIN ��ư�� Ŭ���ϸ� ���̵� ��������� �ٲ�ϴ�. ��Ͻ� ���ÿ��� �н����尡 ������ Ȯ�������� �ʽ��ϴ�.
��� ������ ����� ������ �α������� �̵��Ͽ� �α����� �˴ϴ�.





**��ġ��**

�ٸ� �κ��� �Ű澲�� �ʰ� ���۹��� ���������� �� �� �ֵ��� �� ������ �翬������ ���� �̴�� �������� ���ñ� �ٶ��ϴ�.
���� �α��� ������� �߸��� �α��� ��û�̾��� �� Ʋ������ ���� �� ���� ������ ���°� �Ϲ��� �Դϴ�.
�н����带 ��ȣȭ �Ͽ� ���������� �ʾҽ��ϴ�. ������ ���� ���� � ���� ������ ������ �ϼž� �մϴ�.
