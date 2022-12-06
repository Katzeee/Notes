2022.11.03
---

# what is software architecture

Some keywords

- fundamental organization

- relationships

- all about software design

- focus on issues that difficult to change once the system is built: QA, non-functional requirements

- about structures but not just structures

- components/elements

- a set of principal design decisions

  - architecture = {principal design decisions}

- architecture is **not** a phase of development

- necessary for construction and evolution

## principle design decisions

- aspects: static(interface or something), dynamic

# what is software architect

- is a role not a rank

- make architectural decisions

- continually analyze the architecture

  - strive for consistency

  - organize the code into packages, folder, modules

- keep current with existing trends

  - be aware of the latest technology and industry trends

- be familiar with varying technologies

- teamwork and leadership skills

# what is stakeholders

- (explicitly or implicitly) drive the whole shape and direction of the architecture to serve their needs

# laws of software architecture

- everything in software architecture is a trade-off

- why is more impotent than how

# from architecture drivers(input) to architecture(output)

- drivers: design objectives, functional requirements, **quality attributes**, constraints, concerns

- Non-functional properties

  NFPS are the result of architecture choices

- Quality attributes(=NFP)

  - determine most architectural design decisions

# Quality attributes

- classify:

  - external, internal

  - static, dynamic

  - **runtime, non-runtime**

- it's a trade-off

- some important QAs

  - availability: time possible (201 seconds down/week)

  - reliability: attempts(1 time/1000 attempts)

  - manageability: manage to maintain health(one metric: time of per upgrade consumed)

  - flexibility: change architecture in a cost-effective manner(one metric: independence of codes)

  - performance

  - capacity: run a certain number of jobs per unit time

  -  scalability: the ability to support increasing services

    - vertical: come from adding capacity

    - horizontal: come from adding servers

  - extensibility: modify or add functionality without impacting the existing functionalist(code independence)

  - testability: can determine the results 

- twin peaks model: design to realize

# Why documents

- contributes to architecture design

- clarifies the line between architecture and non-architecture design decisions

- **three uses**: education(to newcomer), communication(among stakeholders), analysis

- goal:

  - main goal: communicate the structure

  - **common vocabulary**

  - describe the software (what and how)

- rules: 

  - avoid unnecessary repetition

  - avoid ambiguity

  - use standard organization or template

  - keep current

# Views and viewpoints

- view: the **consequence**, representation of system

- viewpoint: how to structure the system, the **process**

- number of views depends on stakeholders' needs

# C4 model

- Context, container, components, code

- Container is responsible for components. 

- Container can block the user requests.(AOP)

- `+` connector = C5

# arch42

- from the aspects of problem and solution

- block view: component diagram

- runtime view: sequence diagram

- deployment view: deployment diagram

- architectural decisions: describe why choose this(rationale)

- quality requirements: stimulus vs response

# how to choose QAs

- tech: runtime, non-runtime

- non-tech: business

# 4+1 views

- logical view: for end user, how to use

  - similar to c4 component view
  
  - functional requirements

  - diagram:
  
    - class diagram or component diagram: logical relationships
    
    - state transition diagrams

    - data driven: ER diagrams

  - map use cases onto components

- process view: for system integrator, how to manage, the whole process

  - dynamic aspects of the architecture

  - not all system need this view(such as simple systems)

  - performance, behavior, communication, concurrency

  - this process is not the same as the OS process
  
  - divide into independent tasks:

    - (why?)to distinguish the major tasks(communicate through network) and the minor tasks(through memory)

  - sequence diagram, component diagram

- implementation view: for software management(programmer), how to code

  - static organization of software code

  - map logical view onto code

  - module organization, 'export' and 'import' relationships

- deployment view(physical view): for system engineer

  - non-functional requirements

  - availability, reliability

  - mapping logical to physical entities

- (scenario)use case: the one which deduce other 4 views

  - an abstraction of the most important requirements

  - useful in all circumstance

- 4 views are not fully orthogonal or independent

# typical architectures

## data-flow styles

- properties

  - pros: flexible(reuse, composite), AOP, debug

  - cons: pipe->high latency(cannot interactive), parse/wrap data

- `batch sequential style`

  - sequential

  - example: unix command pipe, old compiler

- `pipe and filter`

  - concurrent

  ```python
  class Filter:
    def __init__(self):
        self.input_pipe = None
        self.output_pipe = None

    def set_input_pipe(self, pipe):
        self.input_pipe = pipe

    def set_output_pipe(self, pipe):
        self.output_pipe = pipe

    def process(self, data):
        pass


  class Pipe:
    def __init__(self):
        self.data = None

    def put(self, data):
        self.data = data

    def get(self):
        return self.data


  class PipeAndFilterArchitecture:
    def __init__(self):
        self.filters = []

    def add_filter(self, filter):
        self.filters.append(filter)

    def connect_filters(self):
        for i in range(len(self.filters) - 1):
            self.filters[i].set_output_pipe(self.filters[i + 1].input_pipe)

    def process(self, data):
        self.filters[0].input_pipe.put(data)

        for filter in self.filters:
            filter.process(filter.input_pipe.get())

        return self.filters[-1].output_pipe.get()

  ```
  In this example, the `Filter` class represents a filter that processes data and passes it to the next filter in the pipeline. 
  The `Pipe` class represents a pipe that connects two filters and holds the data that is passed between them. 
  The `PipeAndFilterArchitecture` class represents the overall pipeline and manages the filters and pipes. 
  The `add_filter` method adds a filter to the pipeline, the `connect_filters` method connects the filters together, and the `process` method processes the data through the pipeline.

## data-centered

- properties

  - centralize data store
  
  - three protocol: communication, definition, data manipulation
  
  - pros: data integrity, secure, performance on client side

  - cons: scalability, unclear border of functionalities

- `repository`

  - clients send request to the system(via system interface)

  - examples: database(clients use DML to work with data), REST architecture

  - A medical diagnosis system that uses a blackboard architecture. In this system, multiple independent knowledge sources, such as a symptom checker, a lab test analysis module, and a medical expert system, operate on the data stored on the blackboard (patient symptoms, lab test results, and medical knowledge) in parallel to generate a diagnosis.

  - code

    ```python
    class Repository:
      def __init__(self):
          self.data = {}
  
      def add(self, key, value):
          self.data[key] = value
  
      def get(self, key):
          return self.data[key]


    class Component:
        def __init__(self, repository):
            self.repository = repository
    
      def process(self, data):
        # Get the current data from the repository
        current_data = self.repository.get('data')
        # Perform some processing on the data
        processed_data = process_data(data)
        # Update the repository with the processed data
        self.repository.add('data', processed_data)
    
    
    class RepositoryArchitecture:
        def __init__(self):
            self.repository = Repository()
            self.components = []
    
        def add_component(self, component):
            self.components.append(component)
            component.repository = self.repository
    
        def process(self, data):
            for component in self.components:
                component.process(data)
    
    ```
    In this example, the Repository class represents a central data store that holds the data that is used by the components in the system. The Component class represents a component that accesses and modifies the data in the repository. The RepositoryArchitecture class manages the repository and the components, and the process method processes the data by calling the process method on each component.

- `blackboard`

  - system sends notification and data to subscriber

  - examples: speech and pattern recognition(complex system), web architecture

  - An online shopping website that uses a repository pattern. In this system, a central data store (the repository) holds all of the product information, and the website's components (the shopping cart, the checkout process, and the order confirmation page) access and modify the data in the repository through well-defined interfaces.

  - code

    ```python
    class Blackboard:
      def __init__(self):
          self.data = {}
          self.subscribers = set()
  
      def add(self, key, value):
          self.data[key] = value
  
          # Notify subscribers that the data has been updated
          for subscriber in self.subscribers:
              subscriber.update(key, value)
  
      def get(self, key):
          return self.data[key]
  
      def subscribe(self, subscriber):
        self.subscribers.add(subscriber)


    class KnowledgeSource:
        def __init__(self, blackboard):
            self.blackboard = blackboard
            self.blackboard.subscribe(self)
    
        def process(self):
            pass
    
        def update(self, key, value):
            # Handle updates from the blackboard
            pass

    ```
    In this example, the Blackboard class now has a subscribers set to hold a list of subscribers, and a subscribe method to add new subscribers. The Knowledge_source class now has an update method to handle updates from the blackboard, and it subscribes to the blackboard when it is initialized. The Blackboard class also calls the update method on each subscriber when the data is updated. This allows knowledge sources to be notified when the data on the blackboard has been changed, so they can take appropriate action.



## layer architecture

- properties

  - connectors are protocols of layer interaction
  
  - example: OS, network, VM(specifies an interface between compiler and a real machine from conceptual point of view)
  
  - satisfy all design principles

  - pros: portability, layers are replaceable, low couple, evolvability, reuse

  - cons: not all system fit, abstraction lead to low performance, hard to find right level of abstraction

- `N-tier`

  - conceptually separate architecture into presentation, application and data storage layers

## Event driven architecture(EDA)

- properties

  - loosely coupled components
  
  - connector is network protocol

  - pros: decouple, timelessness, asynchronous

  - cons: non-sequential execution(no control over process flow), consistency, difficult to debug(testibility)

- `notification architecture(publish-subscribe)`

  - indirect communication cause delay

- `event-based(such as message queue)`

  - components are event generator and consumer

  - connectors are event buses

## network-centered style

- properties

  - components are servers and clients

  - connectors are RPC-based network interaction protocols

  - focus on communication

  - cons: performance(communication slow)

- `clinet-server`

  - SOC(separation of concerns)

  - scale vertically is often the only option

  - `replicated server`

    - pros: fault tolerance, less latency

    - cons: consistency

- `p2p`

  - a bootstrapping mechanism is needed(how to find other peers)

  - centralised peer to peer

    - not all peers are equal, some have additional responsibility

  - pros: scalability, reliability

  - cons: quality of service is not deterministic, high latency

## remote invocation architecture

- properties

  - client invoke method(function) on a remote component

  - pros: increase performance through distributed computation

  - cons: network, tightly couple, addressability increase communication overhead

- `broker(proxy)`

  - separation communication from the application functionality support for distributed systems

  - can be used to transparently change a non-distributed system into a distributed one

  - pros: decouple(network aspect), flexibility, maintainability, evolvability

  - cons: network

## interpreter

- properties

  - components: command interpreter, program state, user interface

  - connectors: direct procedure call, shared state

  - pros: flexibility, usability, adaptability

  - cons: performance, security(wrong programs), design of DSL(domain specified language)

- `embedded DSL`

  - pros: reuse of host language(include syntax and libraries)

  - cons: separation between DSL and host language

## interceptor

- properties:

  - separate functionalities into separate components(SOC), AOP

  - pros: reuse, flexibility, decouple, maintainability, evolvability

  - cons: potential cascading callback, core components need to provide callback interface

## GUI architecture

## Adaptive style

- properties

  - allow extending system using plugins

  - pros: extensibility, customization

  - cons: consistency, performance, security(3rd-party)

- `micro kernel`

  - minimum functionality of internal server

  - pros: security(critical parts are encapsulated)

- `Reflection`

  - has meta information about other elements of a system

  - change the structure and behavior of an application dynamically

  - pros: flexibility

  - cons: low performance, complex

# SOA(service oriented architecture)

- why SOA

  - 2020-2050(properties)

    - distributed functions

    - data centric
  
  - respond to business changes(extensibility)

    - more agile to change business needs

- what is SOA

  - service: a repeatable business task

  - service orientation: a way of integrating your business as linked services

- Characteristics of SOA

  - shared services

  - loose coupling

  - location transparency

  - software infrastructure is responsible for managing

  - stateless

  - **service bus**

- SOA supports Business process management(BPM)

  - interpret architecture(IDL in CORBA, WSDL in Web Service)

  - manage workflow

- CORBA is similar to SOA

  - IDL(describes service)

- service communication

  - message(SOAP protocol)

  - services don't know about others

- service use open standards(describe, communicate, exchange data)

- Web service(one way to realize SOA)

# Microservices

- Why microservices

  - faster and simpler deployment and rollbacks

  - right framework/tool/language for each domain

  - greater resiliency(fault isolation)

- what is microservices

  - fine grain SOA

- three aspects of it

  - technical

  - architectural

    - single responsibility

    - bounded context

    - ...

  - organizational

    - teams

    - you build it you run it

# Serverless

- what is serverless

  - abstraction of servers

  - a cloud-native platform

  - short-running, stateless computation

  - event-driven applications

  - scales up and down instantly and automatically(flexible)

  - charges for actual usage at a millisecond granularity

- server-less means worry-less about servers(just code)

  - focus on business logic 

# MVP

```cpp
// The model represents the data and business logic of the application.
class User {
  int id;
  String name;

  User(this.id, this.name);

  void updateName(String name) {
    this.name = name;
  }
}

// The view represents the user interface of the application.
// It has no knowledge of the model or the presenter, and it only
// communicates with the presenter through a well-defined interface.
abstract class UserView {
  void showUser(User user);
  void showError(String message);
}

// The presenter is responsible for handling the logic of the user
// interface and interacting with the model. It has a reference to
// the view and the model, and it updates the view based on the
// data and events from the model.
class UserPresenter {
  UserView view;
  User model;

  UserPresenter(this.view, this.model);

  void loadUser() {
    try {
      // Load the user from the model and update the view.
      User user = model.load();
      view.showUser(user);
    } catch (e) {
      // Show an error message on the view if there is an exception.
      view.showError(e.message);
    }
  }

  void saveUser(String name) {
    try {
      // Update the user in the model and show the updated user on the view.
      model.updateName(name);
      User user = model.save();
      view.showUser(user);
    } catch (e) {
      // Show an error message on the view if there is an exception.
      view.showError(e.message);
    }
  }
}

```

