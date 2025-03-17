# Pre-Attack (Recon & Resource Development)

# 1-Weaponization

- **Domain Providers:**
  - [domain4bitcoin](https://domain4bitcoin.com) (good option)
  - [hostsalior](https://hostsalior.com) (affordable option)
  - [monovm](https://monovm.com) (best for MikroTik servers)
- **Storage Hosting:**
  - [seedvps](https://seedvps.com)
- **Server Hosting:**
  - [zap-hosting](https://zap-hosting.com)



# 2-Recon

## passive Recon

### Data Sources
- **Fastflux Domain:** Domains from certain countries like Russia and China can help anonymize requests by forwarding them through multiple servers, making it harder to trace the origin.
- **Leak Data Repositories:**
  - [RaidForums](https://raidforums.com) (Find and purchase leaked data)
- **Checking for Leaked Credentials:**
  - Search for organization-related names in leaked data lists.

- **Log Files:**
- Log files are widely used for gathering credentials.
- Many providers crack software and distribute it, collecting logs in return.
- Use tools like **EM Editor** to analyze log files for relevant information.

### Finding Organizational Emails
- [hunter.io](https://hunter.io) (Find organizational emails)
- [snov.io](https://snov.io) (Find organizational emails)
- Once a list of emails is gathered, search on LinkedIn and other platforms for additional patterns.
- Reference: [OSINT Framework](https://osintframework.com)

### Identifying Exposed Services
- Use **[shodan.io](https://shodan.io)** to find internet-exposed services.
- If you need a premium account, use an email associated with an American university.


## active Recon


### Finding a Wider Range of IPs
- Utilize these tools to discover subdomains and associated IPs:
  - [dnsdumpster](https://dnsdumpster.com)
  - [securitytrails](https://securitytrails.com)
  - [crt.sh](https://crt.sh)
  - [viewdns.info](https://viewdns.info)
  - [sublist3rV2](https://github.com/hxlxmj/sublist3rV2)
  - [sublist3rV2](https://github.com/hxlxmj/sublist3rV2)
  - [amass](https://github.com/owasp-amass/amass)

```bash
amass.exe enum -d []
amass.exe enum -ip -d []
amass.exe enum -d [] -active
amass.exe enum -brute -src -d []
```

### Active Port Scanning
- Identify known services running within the target organization.
- Scan **only relevant services** and ports that can provide valuable attack vectors.

- Use multiple weak VPS servers for active scanning to reduce traceability.
- Utilize **[RABITMQ](RabitMQ/)**:
  - Define a job queue in **rabitmq**.
  - Multiple **VPS instances** pull and process scanning tasks in parallel.
  - Each VPS checks IPs and ports from the queue and returns results.

### Extracting Metadata from Documents
- **FOCA (GitHub project)**: Extract metadata from leaked/stored documents to gather usernames and other sensitive information.
- If **NTDS** (Windows Active Directory database) is obtained, it may provide full domain administrator access.

### Sending Trackers for Internal IP Discovery
- **[CanaryToken](https://canarytokens.org)**: Send tracking links via email to discover internal IPs, operating systems, and more.

### Automated Screenshots
- Use **webscreenshot** tools to automate reconnaissance via screenshots of discovered websites.

---
### Disclaimer
This guide is for educational and ethical security research purposes only. Unauthorized access to systems is illegal. Always obtain proper authorization before performing any security assessments.

