# AtividadeSockets

Atividade de Sockets na matéria de computação distribuida.

Este projeto implementa uma aplicação cliente-servidor TCP com protocolo customizado para demonstrar conceitos de programação com sockets em linguagem C.

## Como Compilar e Executar

### Requisitos
- Compilador GCC
- Sistema Unix/Linux (testado no Ubuntu)
- Make

### Compilação

Para compilar o projeto completo:
```bash
make all
```

Para compilar apenas o servidor:
```bash
make server
```

Para compilar apenas o cliente:
```bash
make client
```

Para limpar os arquivos de compilação:
```bash
make clean
```

### Execução

#### Servidor

Executar servidor com porta padrão (8080):
```bash
./server
```

Executar servidor com porta personalizada:
```bash
./server 9000
```

Ou usando o Makefile:
```bash
make run-server          # porta 8080
make run-server-port     # porta 9000
```

#### Cliente

Executar cliente em modo de teste (executa sequência automática de testes):
```bash
./client
```

Executar cliente em modo interativo:
```bash
./client -i
```

Conectar a servidor específico:
```bash
./client -p 9000 -s 192.168.1.100
```

Usando o Makefile:
```bash
make run-client              # modo teste
make run-client-interactive  # modo interativo
make run-client-custom       # conexão personalizada
```

Para ver todas as opções disponíveis:
```bash
./client --help
make help
```

### Exemplo Completo de Uso

1. **Terminal 1 - Servidor:**
```bash
make all
./server
```

2. **Terminal 2 - Cliente (modo teste):**
```bash
./client
```

3. **Terminal 3 - Cliente (modo interativo):**
```bash
./client -i
client> hello
client> echo Olá mundo!
client> calc 10 + 5
client> time
client> bye
```

## Formato do Protocolo

### Estrutura da Mensagem

O protocolo utiliza uma estrutura binária com os seguintes campos:

```c
typedef struct {
    message_type_t type;    // 4 bytes - Tipo da mensagem
    int length;            // 4 bytes - Tamanho dos dados
    char data[1016];       // 1016 bytes - Dados da mensagem
} message_t;
```

### Tipos de Mensagem

| Tipo | Valor | Descrição | Exemplo de Dados |
|------|-------|-----------|------------------|
| MSG_HELLO | 1 | Mensagem de saudação | (vazio) |
| MSG_ECHO | 2 | Eco de mensagem | "Texto para ecoar" |
| MSG_CALC | 3 | Calculadora | "10 + 5" |
| MSG_TIME | 4 | Solicitação de horário | (vazio) |
| MSG_BYE | 5 | Despedida | (vazio) |
| MSG_RESPONSE | 100 | Resposta do servidor | "Resultado da operação" |
| MSG_ERROR | 999 | Erro | "Mensagem de erro" |

### Exemplos de Protocolo

**Exemplo 1: Saudação**
```
Cliente -> Servidor: {type: 1, length: 0, data: ""}
Servidor -> Cliente: {type: 100, length: 37, data: "Hello! Welcome to the socket server!"}
```

**Exemplo 2: Echo**
```
Cliente -> Servidor: {type: 2, length: 11, data: "Olá mundo!"}
Servidor -> Cliente: {type: 100, length: 17, data: "Echo: Olá mundo!"}
```

**Exemplo 3: Calculadora**
```
Cliente -> Servidor: {type: 3, length: 7, data: "15 + 25"}
Servidor -> Cliente: {type: 100, length: 14, data: "Result: 40.00"}
```

**Exemplo 4: Horário**
```
Cliente -> Servidor: {type: 4, length: 0, data: ""}
Servidor -> Cliente: {type: 100, length: 35, data: "Server time: Thu Sep 18 11:16:56 2025"}
```

### Operações Suportadas na Calculadora

- Adição: `+`
- Subtração: `-`
- Multiplicação: `*`
- Divisão: `/`

Formato: `<número1> <operador> <número2>`

Exemplos:
- `10 + 5` → `Result: 15.00`
- `20 - 8` → `Result: 12.00`
- `3 * 7` → `Result: 21.00`
- `15 / 3` → `Result: 5.00`

## Decisões de Projeto e Limitações Conhecidas

### Decisões de Projeto

1. **Protocolo Binário**: Utilizado para eficiência na transmissão de dados estruturados.

2. **Arquitetura Cliente-Servidor TCP**: Escolhido TCP para garantir entrega confiável das mensagens.

3. **Estrutura Modular**: Separação em arquivos `common.c`, `server.c` e `client.c` para facilitar manutenção.

4. **Modo Interativo e Teste**: Cliente oferece ambos os modos para facilitar desenvolvimento e demonstração.

5. **Makefile Completo**: Inclui alvos para compilação, execução e testes automatizados.

6. **Tratamento de Erros**: Implementado verificação de erros em operações de socket.

### Limitações Conhecidas

1. **Concorrência**: O servidor atual atende apenas um cliente por vez (single-threaded).
   - **Impacto**: Clientes adicionais ficam em espera.
   - **Solução futura**: Implementar multi-threading ou fork para múltiplos clientes.

2. **Tamanho Máximo de Mensagem**: Limitado a 1016 bytes de dados por mensagem.
   - **Impacto**: Mensagens muito grandes não são suportadas.
   - **Solução**: Implementar fragmentação de mensagens grandes.

3. **Validação de Entrada**: Validação básica nos dados recebidos.
   - **Impacto**: Dados malformados podem causar comportamento inesperado.
   - **Melhoria**: Implementar validação mais robusta.

4. **Segurança**: Não há autenticação ou criptografia.
   - **Impacto**: Comunicação em texto claro, sem controle de acesso.
   - **Uso**: Adequado apenas para ambientes de desenvolvimento/estudo.

5. **Reconexão Automática**: Cliente não tenta reconectar automaticamente.
   - **Impacto**: Perda de conexão requer reinício manual do cliente.

6. **Log de Atividades**: Log básico apenas no console.
   - **Limitação**: Não há persistência de logs ou rotação de arquivos.

## Como Rodar Testes

### Testes Automáticos

O projeto inclui uma sequência de testes automáticos que pode ser executada:

```bash
# Certifique-se de que o servidor está rodando em outra janela
./server

# Em outro terminal, execute os testes
make test
```

Ou diretamente:
```bash
./client
```

### Testes Manuais

Para testes interativos detalhados:

```bash
./client -i
```

Comandos disponíveis:
- `hello` - Testa mensagem de saudação
- `echo <mensagem>` - Testa eco de mensagem
- `calc <número> <op> <número>` - Testa calculadora
- `time` - Testa solicitação de horário
- `bye` - Testa encerramento de conexão
- `help` - Mostra ajuda

### Script de Teste Completo

```bash
# Terminal 1: Iniciar servidor
make clean && make all
./server

# Terminal 2: Executar todos os testes
echo "=== Teste Automático ==="
./client

echo "=== Teste com Porta Personalizada ==="
# No Terminal 1, pare o servidor (Ctrl+C) e reinicie:
# ./server 9000
./client -p 9000

echo "=== Teste de Opções de Ajuda ==="
./client --help
make help
```

### Verificação de Funcionalidades

✅ **Funcionalidades Testadas:**
- Estabelecimento de conexão TCP
- Envio e recebimento de mensagens estruturadas
- Processamento de diferentes tipos de mensagem
- Operações matemáticas básicas
- Obtenção de horário do servidor
- Encerramento correto da conexão
- Tratamento de erros básicos

## Estrutura do Projeto

```
AtividadeSockets/
├── README.md        # Este arquivo
├── Makefile         # Script de compilação
├── common.h         # Definições compartilhadas
├── common.c         # Funções compartilhadas
├── server.c         # Implementação do servidor
├── client.c         # Implementação do cliente
├── server           # Executável do servidor (após compilação)
└── client           # Executável do cliente (após compilação)
```

## Autor

Projeto desenvolvido como atividade da matéria de Computação Distribuída.