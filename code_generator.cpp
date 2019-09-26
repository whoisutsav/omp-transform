#include <iostream>
#include "code_generator.h"
#include "ast.h"

std::string generateIntLiteral(IntLiteral* intLiteral) {
  return std::to_string(intLiteral->getValue()); 
}

std::string generateVarExpr(VarExpr* varExpr) {
  return varExpr->getName();
}

std::string generateBinaryExpr(BinaryExpr* binaryExpr) {
  return CodeGenerator::generate(binaryExpr->getLHS()) + 
          std::string(1, binaryExpr->getOp()) +
          CodeGenerator::generate(binaryExpr->getRHS());
}

std::string generateExprStmt(ExprStmt* exprStmt) {
  return CodeGenerator::generate(exprStmt->getExpr()) + ";\n";
}

std::string generateCompoundStmt(CompoundStmt* compoundStmt) {
  std::string result = "{\n";
  for(std::vector<Stmt*>::iterator i = compoundStmt->getStatements().begin();
                  i != compoundStmt->getStatements().end();
                  i++) {

    result += CodeGenerator::generate(*i);
  }

  result += "}\n";
  return result;
}

std::string generateReturnStmt(ReturnStmt* returnStmt) {
  return "return " + CodeGenerator::generate(returnStmt->getRetValue()) + ";\n";
}

std::string generateDeclStmt(DeclStmt* declStmt) {
  return "int " + CodeGenerator::generate(declStmt->getVar()) + 
          "=" +
          CodeGenerator::generate(declStmt->getValue()) + 
          ";\n";
}

std::string generateForStmt(ForStmt* forStmt) {
  return "for(" + 
          CodeGenerator::generate(forStmt->getInit()) + 
          CodeGenerator::generate(forStmt->getCond()) + ";" +
          CodeGenerator::generate(forStmt->getInc()) +
          ")\n" + 
          CodeGenerator::generate(forStmt->getBody());
}

std::string generateParamDecl(ParamDecl* paramDecl) {
  return paramDecl->getType() + " " + CodeGenerator::generate(paramDecl->getVar());
}

std::string generateFunctionDecl(FunctionDecl* functionDecl) {
  std::string paramList = "";

  for(int i=0; i<functionDecl->getParams().size(); i++) {
    paramList += CodeGenerator::generate(functionDecl->getParams()[i]);

    if (i != functionDecl->getParams().size() - 1)
      paramList += ", ";
  }

  return functionDecl->getReturnType() + " " +
         functionDecl->getName() + "(" +
         paramList +
         ")\n" +
         CodeGenerator::generate(functionDecl->getBody());
}

std::string CodeGenerator::generate(Node* root) {
  IntLiteral* intLiteral = dynamic_cast<IntLiteral*>(root);
  if (intLiteral) return generateIntLiteral(intLiteral);

  VarExpr* varExpr = dynamic_cast<VarExpr*>(root);
  if (varExpr) return generateVarExpr(varExpr);

  BinaryExpr* binaryExpr = dynamic_cast<BinaryExpr*>(root);
  if (binaryExpr) return generateBinaryExpr(binaryExpr);

  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(root);
  if (exprStmt) return generateExprStmt(exprStmt);

  CompoundStmt* compoundStmt = dynamic_cast<CompoundStmt*>(root);
  if (compoundStmt) return generateCompoundStmt(compoundStmt);

  ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(root);
  if (returnStmt) return generateReturnStmt(returnStmt);

  DeclStmt* declStmt = dynamic_cast<DeclStmt*>(root);
  if (declStmt) return generateDeclStmt(declStmt);

  ForStmt* forStmt = dynamic_cast<ForStmt*>(root);
  if (forStmt) return generateForStmt(forStmt);

  ParamDecl* paramDecl = dynamic_cast<ParamDecl*>(root);
  if (paramDecl)  return generateParamDecl(paramDecl);      

  FunctionDecl* functionDecl = dynamic_cast<FunctionDecl*>(root);
  if (functionDecl) return generateFunctionDecl(functionDecl);

  return "";
}


