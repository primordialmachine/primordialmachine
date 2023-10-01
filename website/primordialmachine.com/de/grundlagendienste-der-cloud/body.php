<?php
defined('SITE_PRIVATE') || die("no direct access");
?>
<h1>Grundlagendienste der Cloud (Stand: 2023-10-23)</h1>
<p>
Ein Cloud-Dienst (z.B. die Bereitstellung von virtuellen    Maschinen)
mit einer Effektivität, wie Kunden Sie von Hyperscalern  gewohnt sind,
wird durch seine Integration mit Grundlagendiensten der          Cloud
ermöglicht.    Grundlagendienste erbringen Leistungen aus    Bereichen
wie z.B. Identität und Berechtigung, Sicherheit, Konformität       und
vielen weiteren Bereichen.

Eine Betrachtung dieser Grundlagendienste ist lohnenswert, wenn eine
eigene Realisierung einer Cloud angestrebt ist, welche Cloud-Dienste
mit annähernd solcher Effektivität zur Verfügung zu stellen   sucht.

Dieser Artikel bietet einen Überblick über   nennenswerte
Grundlagendienste wobei kein Anspruch auf Vollständigkeit
erhoben wird.</p>

<p>
Die folgende Tabelle listet die in diesem Dokument betrachteten
Grundlagendienste auf.<span style="color:red">Beschreibung  der
Spalten bzw. Zeilen.</span>
</p>

<table style="">
  <tr><td>Identity and Access Management (IAM)            </td><td>                             </td></tr>
  <tr><td>                                                </td><td>Role-based Access (RBAC)     </td></tr>
  <tr><td>                                                </td><td>Attribute-based Access (ABAC)</td></tr>
  <tr><td>Information Protection (IP)                     </td><td>                             </td></tr>
  <tr><td>Security Information and Event Management (SIEM)</td><td>                             </td></tr>
  <tr><td>Extended Detection and Response (XDR)           </td><td>                             </td></tr>
  <tr><td>Resource Management Interfaces (RMI)            </td><td>                             </td></tr>
  <tr><td>Fabric                                          </td><td>                             </td></tr>
</table>

<dl>
  <dt>Identity and Access Management (IAM)</dt>
  <dd>
    Identity und Access Management stellt sicher, dass die richtigen Personen bzw. Maschinen
    zum richtigen Zeitpunkt, für die richtige Zeitdauer Zugriff auf die richtigen Ressourcen
    erhalten. <span style="color:red">Beispiele für Produkte.</span>
  </dd>
  <dt>Information Protection (IP)    </dt>
  <dd>
    Information Protection ermöglicht das Entdecken, Klassifizieren, Schützen
    und Verwalten von (vertraulichen) Informationen bei Speicherung       und
    Übertragung. Produkte dieses Bereichs sind hier       z.B. Azure Purview.
  </dd>

  <dt>Security Information and Event Management (SIEM)</dt>
  <dd>
    <p>
    Unsere Verständnis von was SIEM ist folgt
    <a href="https://www.sans.org/white-papers/1781/?show=1781.php&cat=logging">
    "David Swift; 2007; A Practical Application of SIM/SEM/SIEM Automating Threat Identification"
    </a> und umfasst Funktionalitäten für
    </p>
    <ul class="p-list" style="margin-top: -0.5rem">
      <li>die automatisierte(!) Konsolidierung von Protokollinformationen,</li>
      <li>die automatisierte(!) Korrelation von Protokollinformationen (auch
          durch Heuristiken und Künstliche Intelligenz) zur  Indentifikation
          von Bedrohungen, sowie</li>
      <li>Workflows für das Incident Management, und</li>
      <li>Workflows für das Reporting (SOX, HIPPA, FISMA, ...).</li>
    </ul>
    <p>
    Produkte diese Bereichs sind z.B Azure Sentinel &amp; Lighthouse,
    Splunk, Trellix, uvm.)
    </p>
  </dd>

  <dt>Extended Detection and Response (XDR)</dt>
  <dd>
    XDR komplementiert SIEM durch eine Automatisierung der Workflows des Incident Management und des Reportings.
  </dd>

  <dt>Resource Management Interfaces (RMI)</dt>
  <dd>
    <p>
    Resource Management Interface (RMI) ermöglicht die manuelle, halb-, oder vollautomatische Verwaltung von
    Cloud Ressourcen. Beispiele sind Command-Line Interfaces (wie Azure CLI, AWS CLI), REST APIs (wie Azure
    REST API, AWS REST API) oder Web-Portale (wie Azure Portal, AWS Management Console).
    </p>

    <p>
    RMI und insbesondere REST APIs sind eine Voraussetzung für Infrastructure-as-a-Code Werkzeuge,
    die auf solche Schnittstellen zugreifen.
    </p>
  </dd>
  
  <dt>Fabric</dt>
  <dd>
      <p>
      Als das "Fabric" (dt. Gewebe) von Hyperscalern   werden die
      Element bezeichnet, welche           Infrastrukturelementen
      (Server, Datenzentren, ...)      zu einer Cloud verbinden.
      </p>
            
      <p>
      Zwei Kernkonzepte des Fabric sind Virtualisierung und
      Orchestrierung.</p>
      
      <p>
      Virtualisierung ist die Einführung einer Trennschicht
      - einem Hypervisor - zwischen dem    (physikalischem)
      Server und dem Server-Betriebssystem.
     Diese Trennschicht
      simuliert auf dem (physikalischen) Server mehrere
      virtuelle Server. Jeder dieser virtuellen Server
      kann sein eigenes Betriebssystem ausführen.
      </p>

      <p>
      Ein  Rechenzentrum enthält viele solcher Server mit
      Hypervisoren ausgestatteten Server, die in    Racks
      gruppiert (dt. Regale) gruppiert sind.        Jedes
      Rack hat einen  Switch, der eine Verbindung zu  den
      Servern (z.B. per SSH) gestattet. </p>
      
      <p>
      Ein einzelner Server eines Racks führt den sogenannten
      Fabric-Controller aus, der über den Switch    mit dem
      sogenannten Orchestrator verbunden           ist.  Der
      Orchestrator steuert alles, was in der Cloud passiert.
      Insbesondere bietet er eine API-Schnittstelle,  welche
      direkt oder indirekt über Portale                 bzw.
      Kommandozeilenschnittstellen angesprochen werden kann.
      </p>
      
      <p>
      Beispiele für nicht-proprietäre   Lösungen,
      welche die Konstruktion von        Fabrics
      softwareseitig begleitet ist z.B. <a href=
      "https://openstack.org/">OpenStack</a>.
      OpenStack ist besonders Teil des      LOKI
      Stacks (Linux, OpenStack, and    Kubernetes
      Infrastructure).
      </p>
  </dd>
</dl>
