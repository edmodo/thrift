package thrift

// A request object encapsulates an incoming request that must be processed.
type Request interface {
    SeqId() int32
	Name() string
	In() TProtocol
	Out() TProtocol
}

type SimpleRequest struct {
	seqId   int32
	name    string
	in      TProtocol
	out     TProtocol
}

func NewSimpleRequest(seqId int32, name string, in, out TProtocol) *SimpleRequest {
	return &SimpleRequest{
		seqId: seqId,
		name: name,
		in: in,
		out: out,
	}
}

func (m *SimpleRequest) SeqId() int32 {
	return m.seqId
}

func (m *SimpleRequest) Name() string {
	return m.name
}

func (m *SimpleRequest) In() TProtocol {
	return m.in
}

func (m *SimpleRequest) Out() TProtocol {
	return m.out
}
