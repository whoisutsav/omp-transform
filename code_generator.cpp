#include <iostream>
#include "code_generator.h"
#include "ast.h"

std::string getIndent(int indentLevel) {
  return std::string(indentLevel*2, ' ');
}

std::string generateIntLiteral(IntLiteral* intLiteral) {
  return std::to_string(intLiteral->getValue()); 
}

std::string generateVarExpr(VarExpr* varExpr) {
  return varExpr->getName();
}

std::string generateArrayExpr(ArrayExpr* arrayExpr) {
  return CodeGenerator::generate(arrayExpr->getBase()) + 
          "[" + CodeGenerator::generate(arrayExpr->getIndex()) + "]";
}

std::string generateBinaryExpr(BinaryExpr* binaryExpr) {
  return CodeGenerator::generate(binaryExpr->getLHS()) + 
          std::string(1, binaryExpr->getOp()) +
          CodeGenerator::generate(binaryExpr->getRHS());
}

std::string generateCallExpr(CallExpr* callExpr) {
  std::string argList = "";

  for(int i=0; i<callExpr->getArgs().size(); i++) {
    argList += CodeGenerator::generate(callExpr->getArgs()[i]);

    if (i != callExpr->getArgs().size() - 1)
      argList += ", ";
  }

  return callExpr->getName() + "(" + argList + ")";
}

std::string generateExprStmt(int indentLevel, ExprStmt* exprStmt) {
  return getIndent(indentLevel) + CodeGenerator::generate(exprStmt->getExpr()) + ";\n";
}

std::string generateCompoundStmt(int indentLevel, CompoundStmt* compoundStmt) {
  std::string result = "";
  for(std::vector<Stmt*>::iterator i = compoundStmt->getStatements().begin();
                  i != compoundStmt->getStatements().end();
                  i++) {

    result += CodeGenerator::generate(indentLevel, *i);
  }

  return result;
}

std::string generateReturnStmt(int indentLevel, ReturnStmt* returnStmt) {
  return getIndent(indentLevel) + "return " + CodeGenerator::generate(returnStmt->getRetValue()) + ";\n";
}

std::string generateDeclStmt(int indentLevel, DeclStmt* declStmt) {
  return getIndent(indentLevel) + "int " + CodeGenerator::generate(declStmt->getVar()) + 
          "=" +
          CodeGenerator::generate(declStmt->getValue()) + 
          ";\n";
}

std::string generateForInitDeclStmt(DeclStmt* declStmt) {
  return "int " + CodeGenerator::generate(declStmt->getVar()) + 
          "=" +
          CodeGenerator::generate(declStmt->getValue()) + 
          ";";
}


std::string generateForStmt(int indentLevel, ForStmt* forStmt) {
  return getIndent(indentLevel) + "for(" + 
          generateForInitDeclStmt(forStmt->getInit()) + 
          CodeGenerator::generate(forStmt->getCond()) + ";" +
          CodeGenerator::generate(forStmt->getInc()) +
          ") {\n" + 
          CodeGenerator::generate(indentLevel+1, forStmt->getBody()) + 
          getIndent(indentLevel) + "}\n";
}

std::string generateParamDecl(ParamDecl* paramDecl) {
  return paramDecl->getType() + " " + CodeGenerator::generate(paramDecl->getVar());
}

std::string generateFunctionDecl(int indentLevel, FunctionDecl* functionDecl) {
  std::string paramList = "";

  for(int i=0; i<functionDecl->getParams().size(); i++) {
    paramList += CodeGenerator::generate(functionDecl->getParams()[i]);

    if (i != functionDecl->getParams().size() - 1)
      paramList += ", ";
  }

  return getIndent(indentLevel) + functionDecl->getReturnType() + " " +
         functionDecl->getName() + "(" +
         paramList +
         ") {\n" +
         CodeGenerator::generate(indentLevel+1, functionDecl->getBody()) +
         getIndent(indentLevel) + "}\n";
}

std::string generateParallelFor(int indentLevel, ParallelFor* parallelFor) {
  return getIndent(indentLevel) + "#pragma omp parallel for\n" +
          CodeGenerator::generate(indentLevel, parallelFor->getBody());

}

std::string CodeGenerator::generate(Node* node) {
  IntLiteral* intLiteral = dynamic_cast<IntLiteral*>(node);
  if (intLiteral) return generateIntLiteral(intLiteral);

  VarExpr* varExpr = dynamic_cast<VarExpr*>(node);
  if (varExpr) return generateVarExpr(varExpr);

  ArrayExpr* arrayExpr = dynamic_cast<ArrayExpr*>(node);
  if (arrayExpr) return generateArrayExpr(arrayExpr);

  BinaryExpr* binaryExpr = dynamic_cast<BinaryExpr*>(node);
  if (binaryExpr) return generateBinaryExpr(binaryExpr);

  CallExpr* callExpr = dynamic_cast<CallExpr*>(node);
  if (callExpr) return generateCallExpr(callExpr);

  ParamDecl* paramDecl = dynamic_cast<ParamDecl*>(node);
  if (paramDecl)  return generateParamDecl(paramDecl);      

  return "";
}

std::string CodeGenerator::generate(int indentLevel, Node* node) {
  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(node);
  if (exprStmt) return generateExprStmt(indentLevel, exprStmt);

  CompoundStmt* compoundStmt = dynamic_cast<CompoundStmt*>(node);
  if (compoundStmt) return generateCompoundStmt(indentLevel, compoundStmt);

  ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(node);
  if (returnStmt) return generateReturnStmt(indentLevel, returnStmt);

  DeclStmt* declStmt = dynamic_cast<DeclStmt*>(node);
  if (declStmt) return generateDeclStmt(indentLevel, declStmt);

  ForStmt* forStmt = dynamic_cast<ForStmt*>(node);
  if (forStmt) return generateForStmt(indentLevel, forStmt);

  FunctionDecl* functionDecl = dynamic_cast<FunctionDecl*>(node);
  if (functionDecl) return generateFunctionDecl(indentLevel, functionDecl);

  ParallelFor* parallelFor = dynamic_cast<ParallelFor*>(node);
  if (parallelFor) return generateParallelFor(indentLevel, parallelFor);

  return CodeGenerator::generate(node);
}


