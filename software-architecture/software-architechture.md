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

  - runtime, non-runtime

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


- twin peaks model

C4 model

Container is responsible for components. 

Container can block the user requests.

+ connector = C5

4+1 views

logical view: for end user, how to use
  - similar to c4 component view
  - functional requirements
  - class diagram or component diagram

process view: for system integrator, how to manage, the whole process

  - performance, behavior
  - divide into independent tasks


implementation view: for software management, how to code

  - static organization of software code

deployment view(physical view): for system engineer, 

  - availability, reliability

  - mapping logical to physical


use case: the one which 

  - an abstraction of the most important requirements

